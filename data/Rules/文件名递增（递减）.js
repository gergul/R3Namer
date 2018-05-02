RN.PARAMS.push({
	"StartNums" : "0,0",
	"StartNums_DESC" : "开始数字",
	"Format" : "{NAME}({0})({1})",
	"Format_DESC" : "{0}->StartNums[0]；{1}->StartNums[1]；{2}->StartNums[2]……\n{NAME}->文件名；{EXT}->扩展名",
	
	"目标类型"			: 0,
	"目标类型_DESC"		: "目标类型：0->同时应用文件和文件夹（默认）；1->只应用文件；2->只应用文件夹"
});

RN.RULES.push(function(filePath, isFile, params)
{	
	if (params["Format"] == "" || !isFile)
		return "";
		
	var fileName = params["Format"];
	fileName = fileName.replace(/\{NAME\}/g, RN.GetFileNameWithoutEx(filePath));
	fileName = fileName.replace(/\{EXT\}/g, RN.GetFileExtension(filePath));
	
	var reg = /[\{][\d+][\}]/g;
	var matchs = params["Format"].match(reg);
	
	var arrStartNums = params["StartNums"].split(',');
	var values = [];
	for (var p in matchs)
	{
		var k = p.replace(/\{/g, "").replace(/\}/g, "");
		values[parseInt(k)] = arrStartNums[parseInt(k)];
	}
	
	fileName = fileName.format(values);
		
	var newFilePath = RN.GetPath(filePath) + "\\" + fileName;
	
	var fileExt = RN.GetFileExtension(filePath);
	if (fileExt != "")
		newFilePath += "." + fileExt;
	
	params["StartNums"] = "";
	for (var i=0; i<arrStartNums.length; ++i)
	{
		params["StartNums"] += (parseInt(arrStartNums[i]) + 1).toString();
		if (i != arrStartNums.length-1)
			params["StartNums"] += ",";
	}
		
	return newFilePath;
});