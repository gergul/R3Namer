RN.PARAMS.push({
	"目标类型"			: 0,
	"目标类型_DESC"		: "目标类型：0->同时应用文件和文件夹（默认）；1->只应用文件；2->只应用文件夹"
});

RN.RULES.push(function(filePath, isFile, params)
{
	if ( (params["目标类型"] != 0 && ((params["目标类型"]==1&& !isFile) || (params["目标类型"]==2&& isFile))) ||
		 (params["目标类型"] != 0 && params["目标类型"] != 1 && params["目标类型"] != 2) )
		return "";
	
	var fileName = RN.GetFileName(filePath);
	var newFileName = escape(fileName);
	var newFilePath = RN.GetDir(filePath) + newFileName;
	return newFilePath;
});