
module(...,package.seeall)
local cache = ""
local longitude,latitude

-- �ȷַָ�ʽת��Ϊ�ȸ�ʽ
function dmmtod(strdmm)
	local strd
	local n = string.find(strdmm,"%.")

	if not n then return end

	local d,m,mm = string.sub(strdmm,1,n-3),string.sub(strdmm,n-2,n-1),string.sub(strdmm,n+1,-1)

	-- �ֵ�С�����ֲ���5λ
	if string.len(mm) < 5 then
		mm = mm..string.rep("0",5-string.len(mm))
	end

	-- ����ת��Ϊ��
	m = tonumber(m..mm)/60

	strd = d.."."..string.format("%0"..string.len(mm).."d",m)

	return strd
end

function googlepos()
	if longitude and longitude ~= "" and latitude and latitude ~= "" then
		print("GPSRAW:",longitude,latitude)
		return "GPS����(��γ,����):"..dmmtod(longitude)..","..dmmtod(latitude)
	else
		return "δ��λ"
	end
end

local function parse(s)
	-- print("gps:",s)
	local head = string.sub(s,1,6)

	if head == "$GPGGA" then
		longitude,latitude = string.match(s,"$GPGGA,[%d%.]+,([%d%.]+),N,([%d%.]+),E")
	end
end

local function recv()
	local s
	repeat
		s = uart.read(2,"*l")
		cache = cache .. s
		if string.find(s,"\n",-2) then
			parse(cache)
			cache = ""
		end
	until string.len(s) == 0
end

sys.reguart(2,recv)
-- 2������ѯһ��
--sys.timer_start(recv,2000)
uart.setup(2,9600,8,uart.PAR_NONE,uart.STOP_1)
