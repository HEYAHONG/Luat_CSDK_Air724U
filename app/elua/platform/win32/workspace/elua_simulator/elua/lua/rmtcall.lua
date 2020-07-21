-- �绰Զ�̿���
require"sys"
require"cc"
require"nvm"

module(...,package.seeall)

local getime = os.time
local lastnum = ""
local lastime = 0
local calltimes = 0

local function changemode(num)
	local result,optext
	if defense.getstate() == "DISARM" or defense.getstate() == "DISARMING" then
		result = defense.alert("call")
		optext = "����"
	else
		result = defense.disarm("call")
		optext = "����"
	end

	if result == true then
		sms.send(num,common.binstohexs(common.gb2312toucs2be(optext.."�ɹ�")))
	end
end

local function checkinterval()
	local currtime = getime()
	local ret = false

	if currtime - lastime <= nvm.get("incallinterval")/1000 then
		ret = true
	end

	lastime = currtime

	return ret
end

local function incall(id,num)
	cc.hangup()

	if nvm.isadmin(num) == false then
		print("invalid num",num)
		return
	end

	if lastnum == "" or lastnum ~= num or lastime == 0 then -- ��ͬ����,���ü���
		lastnum = num
		lastime = getime()
		calltimes = 1
	else -- ��ͬ����,����
		if checkinterval() == true then
			calltimes = calltimes+1
		else
			calltimes = 1
		end
	end

	if calltimes >= nvm.get("incalltimes") then
		--���ò��л�ģʽ
		calltimes = 0
		lastnum = ""
		lastime = 0
		changemode(num)
	end
end
sys.regapp(incall,"CALL_INCOMING")
