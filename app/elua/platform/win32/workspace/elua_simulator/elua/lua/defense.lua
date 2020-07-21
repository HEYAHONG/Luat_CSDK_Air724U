-- ��������
require"sys"
require"misc"
require"pinop"
require"led"
module(...,package.seeall)
local pinset = pinop.pinset
local pinget = pinop.pinget

-- β�俪�����Զ��ر�ʱ��
local AUTO_CLOSE_TRUNK = 5000

-- �����������źŴ���
local ALERT_LS_TIME,DISARM_LS_TIME = 2,1

-- ��ʼ�����
local ready = false

-- ACC״̬
local ACC_ON = pinget(pins.def.ACC_IN_INT)
-- ������ʻ״̬
local carun = false

-- ��������
local DEFSIDE = 1 -- ����
local DEFTRUNK = 2 -- ����
local DEFSHAKE = 3 -- �񶯷���
local DEF4 = 4 -- ����
local DEF5 = 5 -- ���
local DEFUEL = DEF4 -- ����
local DEFEPOWER = 99 -- ������ƿ

--�¼����� ���� ���� ��ʱ ����
local EVT_ALERT = 1
local EVT_DISARM = 2
local EVT_TIMEOUT = 3
local EVT_ALARM_STOP = 4
local EVT_ACC = 5 -- ACC�źű仯
local EVT_TRIG = 0x100
local EVT_SIDE = EVT_TRIG+DEFSIDE
local EVT_WDEF = EVT_TRIG+0x80

--[=[
״̬����
DISARM:����״̬
ALERT:����״̬
DISARMING:������
ALERTING:������
ALARM:������
--]=]
local currstate = "DISARM"

-- �п���
local lock = -1
-- �����п�������
local function locktimeout()
	pinset(false,pins.def.LOCK1_EN)
	pinset(false,pins.def.LOCK2_EN)
end

local function setlock(val)
	if lock == val then return end

	lock = val

	if lock == true then
		-- ����
		pinset(false,pins.def.LOCK2_EN)
		pinset(true,pins.def.LOCK1_EN)
	else
		-- ����
		pinset(false,pins.def.LOCK1_EN)
		pinset(true,pins.def.LOCK2_EN)
	end
	-- ����1.5����������п�������
	sys.timer_start(locktimeout,1500)
end

-- ��˸
local times = 0
local total = 0

function blinkon(mode)
	pinset(true,pins.def.LR_LIGHT_EN)
	if mode == "lightsound" then
		pinset(true,pins.def.ALERT_EN)
	end
	sys.timer_start(blinkoff,500,mode)
end

function blinkoff(mode)
	pinset(false,pins.def.LR_LIGHT_EN)
	if mode == "lightsound" then
		pinset(false,pins.def.ALERT_EN)
	end
	times = times+1
	if times >= total then
		times = 0
		total = 0
	else
		sys.timer_start(blinkon,500,mode)
	end
end

local function slstop()
	sys.timer_stop(slstop)
	sys.timer_stop(blinkon)
	sys.timer_stop(blinkoff)
	pinset(false,pins.def.ALERT_EN)
	pinset(false,pins.def.LR_LIGHT_EN)
end

local function slstart(val)
	slstop()
	times = 0
	total = val
	if val > 1000 then
		sys.timer_start(slstop,val)
		pinset(true,pins.def.ALERT_EN)
		blinkon("light")
	else
		blinkon("lightsound")
	end
end

-- ������
-- �������״̬
local function getdefstatus()
	local tstatus = {}

	tstatus[DEFEPOWER] = not pinget(pins.def.EPOWER)

	tstatus[DEFSHAKE] = pinget(pins.def.SHAKE_IN_INT)
	tstatus[DEFSIDE] = pinget(pins.def.SIDE_IN_INT)
	-- β���������Ϊ����
	if nvm.get("def2valid") == "����" then
		tstatus[DEFTRUNK] = false
	else
		tstatus[DEFTRUNK] = pinget(pins.def.TRUNK_IN_INT)
	end

	pinset(false,pins.def.VOLT_CHOICE)
	rtos.sleep(10)
	if nvm.get("def4stat") == "��·" then
		tstatus[DEF4] = pinget(pins.def.SHORT_ALARM)
	else
		tstatus[DEF4] = pinget(pins.def.PLOUGH_ALARM)
	end

	pinset(true,pins.def.VOLT_CHOICE)
	rtos.sleep(10)
	if nvm.get("def5stat") == "��·" then
		tstatus[DEF5] = pinget(pins.def.SHORT_ALARM)
	else
		tstatus[DEF5] = pinget(pins.def.PLOUGH_ALARM)
	end

	return tstatus
end

local defstatus = {}

local alertnoshake = false
local onlyfuel = false

local function checkstatus(t)
	local talarm = {}

	-- ���������ƿ
	if t[DEFEPOWER] == true then print("EPOWER not ready") return DEFEPOWER end --�����쳣ֻ������

	-- ����������
	if onlyfuel == true then print("DEFUEL not ready") return t[DEFUEL] == true and DEFUEL or true end

	-- ����񶯷���
	if alertnoshake == false and t[DEFSHAKE] == true then print("DEFSHAKE not ready") table.insert(talarm,DEFSHAKE) end

	if onlyfuel ~= true then
		-- �����������
		for k,v in pairs(t) do
			if k ~= DEFSHAKE and k ~= DEFEPOWER and k ~= DEFUEL and v == true then
				if nvm.get("alarmtext",k) ~= "" then -- ֻ�����б�����Ϣ�ķ���,�ޱ�����Ϣ������Ϊ��Ч����
					print("DEF not ready:",k)
					table.insert(talarm,k)
				end
			end
		end
	end

	if #talarm == 0 then
		-- �޷����쳣
		return true
	elseif #talarm == 1 then
		-- ֻ��һ�������쳣,ֱ����ʾ�쳣����
		return talarm[1]
	else
		-- �ж�������쳣,���������쳣����
		return talarm
	end
end

local function check()
	local t = getdefstatus()

	for k,v in pairs(t) do
		if defstatus[k] ~= v then
			defstatus[k] = v
			-- �������� ���
			if v == false then
				t[k] = nil
			end
		else
			t[k] = nil
		end
	end

	return checkstatus(t)
end

local function fsmtimeout()
	procevt(EVT_TIMEOUT)
end

-- �����ɹ�
local function alertsucc()
	sys.timer_stop(fsmtimeout)
	-- �����ɹ������ź���ʾ
	slstart(ALERT_LS_TIME)
	currstate = "ALERT"
	alertnoshake = false
	led.work("alertind")
end

-- ����
local function procdisarm(user,mode)
	-- ����ʱ�������״̬
	defstatus = {}

	-- ֻ������
	if mode == "ALERT_NO_SHAKE" then
		alertnoshake = true
		slstart(ALERT_LS_TIME)
		return
	else
		alertnoshake = false
	end

	led.work("idle")
	-- ����ʱ�п�������
	setlock(false)
	-- �رնϵ����
	pinset(true,pins.def.OIL_EN)
	pinset(true,pins.def.ENGINE_EN)
	alarm.stop("disarm")
	slstart(DISARM_LS_TIME) -- �����ɹ������ź�

	-- �κη�ʽ�ĳ�����Ҫ���������Ĵ���
	if currstate == "ALERT" or currstate == "DISARMING" then -- ֻ�ڲ������ߴ�����״̬�²����������Ĵ���
		-- ����������
		currstate = "DISARMING"
		sys.timer_start(fsmtimeout,nvm.get("delayautoalert"))
	else
		currstate = "DISARM"
	end

	return true
end

-- ����
local function procalert(user,mode)
	if mode == "ONLY_FUEL" then
		--ֻ��������
		onlyfuel = true
	else
		onlyfuel = false
	end

	setlock(true)

	-- ���䵥������ʱ ��Ӱ���п������ϵ���Ϳ���
	if onlyfuel == false then -- �����Ƿ񲼷��ɹ���Ҫ�����п���������
		-- �ϵ����
		pinset(false,pins.def.OIL_EN)
		pinset(false,pins.def.ENGINE_EN)
	end

	if check() == true then
		-- ȫ�������ͨ��
		alertsucc()
		return true
	end

	-- �����ź�20����ʾ��������δ׼����
	slstart(20)
	sys.timer_start(fsmtimeout,nvm.get("alertdelaytime"))

	currstate = "ALERTING"
	return false
end

local function dodisarm(evt,user,mode)
	if evt == EVT_ALERT then
		return procalert(user,mode)
	elseif evt == EVT_DISARM then
		-- ң�����ظ�������Ҫ������Ӧ������ʾ
		if user == "controller" then
			return procdisarm(user,mode)
		else
			return true
		end
	else
		return false
	end
end

local function doalerting(evt,user,mode)
	if evt == EVT_ALERT then -- ��ʱ������״̬��������ظ������Ĵ���
		return procalert()
	elseif evt == EVT_TIMEOUT then
		-- ��ʱ���, �����ɹ�����ʧ��
		local result = checkstatus(defstatus)
		if result == true then
			alertsucc()
		else
			-- ���벼��״̬
			currstate = "ALERT"
			led.work("alertind")
			alertnoshake = false
			-- ��������
			alarm.start(result)
		end
	elseif evt > EVT_WDEF then
		-- ���߷�������
	elseif evt > EVT_TRIG then
		-- ����״̬�仯����
		check() -- ���·���״̬
		if checkstatus(defstatus) == true then
			alertsucc()
		end
	elseif evt == EVT_DISARM then
		sys.timer_stop(fsmtimeout)
		procdisarm(user,mode)
	end
	return true
end

local function gotoalarm1(defid)
	currstate = "ALARM1"
	slstart(20000) -- 20�������ź�
	sys.timer_start(fsmtimeout,15000) -- 15��������
	-- ������������
	alarm.start(defid)
end

local function doalert(evt,user,mode)
	if evt == EVT_ALERT then
		-- �ظ�������Ҫ������Ӧ������ʾ
		return procalert(user,mode)
	elseif evt > EVT_WDEF then
		-- ���߷�������
		gotoalarm1(evt-EVT_WDEF)
	elseif evt > EVT_TRIG then
		local result = check()
		if result ~= true then
			gotoalarm1(result)
		end
	elseif evt == EVT_DISARM then
		procdisarm(user,mode)
	end
	return true
end

local function gotoalarm2(defid)
	-- 15�����ٴδ�������
	currstate = "ALARM2"
	-- ��ʱ���������źų���40��
	slstart(40000)
	sys.timer_start(fsmtimeout,40000)
	alarm.start(defid)
end

local function doalarm1(evt,user,mode)
	if evt > EVT_WDEF then
		-- ���߷�������
		gotoalarm2(evt-EVT_WDEF)
	elseif evt > EVT_TRIG then
		local result = check()
		if result ~= true then
			gotoalarm2(result)
		end
	elseif evt == EVT_TIMEOUT then
		-- ��ʱ���ز���״̬
		currstate = "ALERT"
	elseif evt == EVT_DISARM then
		procdisarm(user,mode)
	end
	return true
end

local function alarmdef(defid)
	-- ��ʱ�κδ������������źų���40��
	slstart(40000)
	sys.timer_start(fsmtimeout,40000)
	alarm.start(defid)
end

local function doalarm2(evt,user,mode)
	if evt > EVT_WDEF then
		-- ���߷�������
		alarmdef(evt-EVT_WDEF)
	elseif evt > EVT_TRIG then
		local result = check()
		if result ~= true then
			alarmdef(result)
		end
	elseif evt == EVT_TIMEOUT then
		-- ��ʱ���ز���״̬
		currstate = "ALERT"
	elseif evt == EVT_DISARM then
		procdisarm(user,mode)
	end
	return true
end

local function dodisarming(evt,user,mode)
	if evt == EVT_TIMEOUT or evt == EVT_ALERT then
		-- ��ʱ,����δ��,����
		return procalert(user,mode)
	elseif evt > EVT_TRIG then
		if pinget(pins.def.SIDE_IN_INT) == true then
			-- ���Ŵ�, ����
			currstate = "DISARM"
			sys.timer_stop(fsmtimeout)
		end
	elseif evt == EVT_DISARM then
		-- ң�����ظ�������Ҫ������Ӧ������ʾ
		return procdisarm(user,mode)
	elseif evt == EVT_ACC then
		-- ������, ACC�����볷��״̬
		if ACC_ON == true then
			currstate = "DISARM"
			sys.timer_stop(fsmtimeout)
		end
	end

	return true
end

local fsm =
{
	{state="DISARM",		evt=EVT_ALERT,			action=dodisarm},
	{state="DISARM",		evt=EVT_DISARM,			action=dodisarm},

	{state="ALERTING",		evt=EVT_TRIG,			action=doalerting},
	{state="ALERTING",		evt=EVT_TIMEOUT,		action=doalerting},
	{state="ALERTING",		evt=EVT_DISARM,			action=doalerting},

	{state="ALERT",			evt=EVT_ALERT,			action=doalert},
	{state="ALERT",			evt=EVT_TRIG,			action=doalert},
	{state="ALERT",			evt=EVT_DISARM,			action=doalert},

	{state="ALARM1",		evt=EVT_TRIG,			action=doalarm1},
	{state="ALARM1",		evt=EVT_TIMEOUT,		action=doalarm1},
	{state="ALARM1",		evt=EVT_DISARM,			action=doalarm1},

	{state="ALARM2",		evt=EVT_TRIG,			action=doalarm2},
	{state="ALARM2",		evt=EVT_TIMEOUT,		action=doalarm2},
	{state="ALARM2",		evt=EVT_DISARM,			action=doalarm2},

	{state="DISARMING",		evt=EVT_ALERT,			action=dodisarming},
	{state="DISARMING",		evt=EVT_TIMEOUT,		action=dodisarming},
	{state="DISARMING",		evt=EVT_TRIG,			action=dodisarming},
	{state="DISARMING",		evt=EVT_DISARM,			action=dodisarming},
	{state="DISARMING",		evt=EVT_ACC,			action=dodisarming},
}

function procevt(evt,user,mode)
	--  δ��ʼ����ɲ��������
	if ready == false then return false end

	-- ACC����ʱң�������ƴ���
	if ACC_ON == true and user == "controller" then
		if evt == EVT_DISARM then -- ң��������ִ���κγ�������
		elseif evt == EVT_ALERT and mode == "ONLY_FUEL" then -- ң�������Ե�����������
		else
			print("acc is on, cannot control")
			return false
		end
	end

	print("FSM:",currstate,evt,user,mode)

	for k,v in pairs(fsm) do
		if currstate == v.state then
			if v.evt == evt then
				return v.action(evt,user,mode)
			elseif v.evt == EVT_TRIG and evt > v.evt then
				return v.action(evt,user,mode)
			end
		end
	end
end

function getstate()
	return currstate
end

function alert(user)
	if user == "controller" and ACC_ON == true then setlock(true) return end -- ��ʻ�а����������п�������

	return procevt(EVT_ALERT,user)
end

function disarm(user)
	if user == "controller" and carun == true then setlock(false) return end -- ��ʻ�а���������п�������

	return procevt(EVT_DISARM,user)
end

function alarmstop(user)
	return procevt(EVT_ALARM_STOP,user)
end

function noshake()
	procevt(EVT_DISARM,"controller","ALERT_NO_SHAKE")
end

function fueltank()
	procevt(EVT_ALERT,"controller","ONLY_FUEL")
end

local function closetrunk()
	pinset(false,pins.def.TRUNK_OUT_EN)
end

function opentrunk()
	pinset(true,pins.def.TRUNK_OUT_EN)
	sys.timer_start(closetrunk,AUTO_CLOSE_TRUNK)
end

local function delaytrig(evt)
	procevt(evt)
end

local function trigevt(evt)
	local t = getdefstatus()

	-- ������ͻȻ�����������������쳣 ��ô��ʱ����Ƿ�EPOWER�ϵ�
	for k,v in pairs(t) do
		if defstatus[k] == nil then t = {} break end

		if v == false or defstatus[k] == v then
			t[k] = nil
		end
	end

	t = checkstatus(t)

	if type(t) == "table" and #t > 1 then
		sys.timer_start(delaytrig,2000,evt)
		return
	end

	procevt(evt)
end

local DELAY_TRIG = 800 -- ��ʱ800ms�������״̬

local function trunkind()
	sys.timer_start(trigevt,DELAY_TRIG,EVT_TRIG+DEFTRUNK)
end

local function sideind()
	sys.timer_start(trigevt,DELAY_TRIG,EVT_TRIG+DEFSIDE)
end

local function shakeind()
	sys.timer_start(trigevt,DELAY_TRIG,EVT_TRIG+DEFSHAKE)
end

-- ������ƿ״̬
local function epowerind(status)
	sys.timer_start(trigevt,DELAY_TRIG,EVT_TRIG+DEFEPOWER)
end

-- �п�������
local acctimeout = false

local function brakeind(status)
	if ACC_ON == true and status == true then
		-- 15������ɲ���Զ�����
		if acctimeout == true then
			acctimeout = false
			carun = true
			setlock(true) -- ������ʻ�Զ�����
		end
	end
end

local function acctimer()
	acctimeout = true
end

local function accind(status)
	ACC_ON = status

	if status == true then
		acctimeout = false
		sys.timer_start(acctimer,15000)
	else
		sys.timer_stop(acctimer)
		acctimeout = false
		carun = false
		setlock(false) -- ����Ϩ���Զ�����
	end
	procevt(EVT_ACC)
end

-- ����4 5 ����������
local function def45timer()
	sys.timer_start(def45timer,2000)

	if currstate == "DISARM" then return end

	local def,status

	def = pins.def.VOLT_CHOICE.val == true and DEF5 or DEF4

	local trigstat = nvm.get(def == DEF4 and "def4stat" or "def5stat")
	status = pinget(trigstat == "��·" and pins.def.SHORT_ALARM or pins.def.PLOUGH_ALARM)

	pinset(not pins.def.VOLT_CHOICE.val,pins.def.VOLT_CHOICE)

	if defstatus[def] ~= status then -- ״̬�����仯�Ŵ���
		sys.timer_start(trigevt,DELAY_TRIG,EVT_TRIG+def)
	end
end

sys.timer_start(def45timer,2000)

local defind = {
	PIN_TRUNK_IND = trunkind,
	PIN_BRAKE_IND = brakeind,
	PIN_SIDE_IND = sideind,
	PIN_ACC_IND = accind,
	PIN_SHAKE_IND = shakeind,
	PIN_EPOWER_IND = epowerind,
}

sys.regapp(defind)

function wdeftrig(id)
	procevt(EVT_WDEF+id)
end

local function devready()
	ready = true
	led.work("readyind",3)
end

-- �п�ʱ�Զ��ų�ʼ�����Ϊ��ʼ��������־
local function smsready()
	devready()
	sys.deregapp(smsready)
end
sys.regapp(smsready,"SMS_READY")

-- �޿�ʱ�ȴ�2����Ϊ��ʼ��������־
local function simind(msgid,simstat)
	if simstat == "NIST" then
		sys.deregapp(smsready)
		sys.timer_start(devready,60000)
	end
	sys.deregapp(simind)
	return true
end
sys.regapp(simind,"SIM_IND")

led.work("initing")
