
local base = _G
local table = require"table"
local rtos = require"rtos"
local uart = require"uart"
module("sys")

local print = base.print
local unpack = base.unpack
local ipairs = base.ipairs
local type = base.type
local pairs = base.pairs
local assert = base.assert

-- ˢ�´���
local refreshflag = false
function refresh()
	refreshflag = true
end

-- ��ʱ������,�Զ����䶨ʱ��id
local uniquetid = 0
local tpool = {}
local para = {}

local function timerfnc(tid)
	if tpool[tid] ~= nil then
		if para[tid] ~= nil then
			tpool[tid](unpack(para[tid]))
			para[tid] = nil
		else
			tpool[tid]()
		end
		tpool[tid] = nil
	end
end

function timer_start(fnc,ms,...)
	assert(fnc ~= nil,"timer_start:callback function == nil")
	timer_stop(fnc)
	uniquetid = uniquetid + 1
	tpool[uniquetid] = fnc
	rtos.timer_start(uniquetid,ms)
	if arg.n ~= 0 then
		para[uniquetid] = arg
	end
	return uniquetid
end

function timer_stop(val)
	if type(val) == "number" then
		tpool[val] = nil
		para[val] = nil
	elseif type(val) == "function" then
		for k,v in pairs(tpool) do
			if v == val then
				rtos.timer_stop(k)
				tpool[k] = nil
				para[k] = nil
				break
			end
		end
	end
end

-- ��ʼ��
function init(mode)
	uart.setup(uart.ATC,0,0,uart.PAR_NONE,uart.STOP_1)
	print("poweron reason:",rtos.poweron_reason())
	-- ģʽ1:��翪��������gsm
	if mode == 1 then
		if rtos.poweron_reason() == rtos.POWERON_CHARGER then
			rtos.poweron(0)
		end
	end
end

-- ����gsm
function poweron()
	rtos.poweron(1)
end

--Ӧ����Ϣ�ַ�,��Ϣ֪ͨ
local apps = {}

-- ֧�����ַ�ʽע��:
-- 1. app�����
-- 2. ������,id1,id2,...,idn
function regapp(...)
	local app = arg[1]

	if type(app) == "table" then
	elseif type(app) == "function" then
		app = {unpack(arg,2,arg.n),procer = arg[1]}
	else
		error("unknown app type "..type(app),2)
	end
	dispatch("SYS_ADD_APP",app)
	return app
end

function deregapp(id)
	dispatch("SYS_REMOVE_APP",id)
end

local function addapp(app)
	-- ����β��
	table.insert(apps,#apps+1,app)
end

local function removeapp(id)
	for k,v in ipairs(apps) do
		if type(id) == "function" then
			if v.procer == id then
				table.remove(apps,k)
				return
			end
		elseif v == id then
			table.remove(apps,k)
			return
		end
	end
end

local function callapp(msg)
	local id = msg[1]

	if id == "SYS_ADD_APP" then
		addapp(unpack(msg,2,#msg))
	elseif id == "SYS_REMOVE_APP" then
		removeapp(unpack(msg,2,#msg))
	else
		local app
		for i=#apps,1,-1 do
			app = apps[i]
			if app.procer then --����ע�᷽ʽ��app,��id֪ͨ
				for _,v in ipairs(app) do
					if v == id then
						if app.procer(unpack(msg)) ~= true then
							return
						end
					end
				end
			elseif app[id] then -- �����ʽ��app,����id֪ͨ
				if app[id](unpack(msg,2,#msg)) ~= true then
					return
				end
			end
		end
	end
end

--�ڲ���Ϣ����
local qmsg = {}

function dispatch(...)
	table.insert(qmsg,arg)
end

local function getmsg()
	if #qmsg == 0 then
		return nil
	end

	return table.remove(qmsg,1)
end

local refreshmsg = {"MMI_REFRESH_IND"}

local function runqmsg()
	local inmsg

	while true do
		inmsg = getmsg()
		if inmsg == nil then
			if refreshflag == true then
				refreshflag = false
				inmsg = refreshmsg
			else
				break
			end
		end

		callapp(inmsg)
	end
end

-- ��Ϣ����
local handlers = {}
base.setmetatable(handlers,{__index = function() return function() end end,})

function regmsg(id,handler)
	handlers[id] = handler
end

local uartprocs = {}

function reguart(id,fnc)
	uartprocs[id] = fnc
end

function run()
	local msg

	while true do
		runqmsg()

		msg = rtos.receive(rtos.INF_TIMEOUT)

		if msg.id == rtos.MSG_TIMER then
			timerfnc(msg.timer_id)
		elseif msg.id == rtos.MSG_UART_RXDATA and msg.uart_id == uart.ATC then
			handlers.atc()
		else -- ͨ����Ϣidע��
			if msg.id == rtos.MSG_UART_RXDATA then
				if uartprocs[msg.uart_id] ~= nil then
					uartprocs[msg.uart_id]()
				else
					handlers[msg.id](msg)
				end
			else
				handlers[msg.id](msg)
			end
		end
		--print("mem:",base.collectgarbage("count"))
	end
end
