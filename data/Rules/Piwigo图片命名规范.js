RN.PARAMS.push({	
	"替换后为空名仍然进行"		: true, 
	"替换后为空名仍然进行_DESC"	: "如果替换后文件名为空时，是否进行重命名（文件名为随机数）。",
	
	"目标类型"					: 0,
	"目标类型_DESC"				: "目标类型：0->同时应用文件和文件夹（默认）；1->只应用文件；2->只应用文件夹"
});

RN.RULES.push(function(filePath, isFile, params)
{	
	if ( (params["目标类型"] != 0 && ((params["目标类型"]==1&& !isFile) || (params["目标类型"]==2&& isFile))) ||
		 (params["目标类型"] != 0 && params["目标类型"] != 1 && params["目标类型"] != 2) )
		return "";

	if (isFile)
	{
		var ex = RN.GetFileExtension(filePath).toUpperCase();
		if (ex != "JPG" && ex != "PNG" && ex != "BMP" && ex != "GIF" && ex != "JPEG")
			return "";
	}
	
	var newFileName = "";
	
	var fileName = RN.GetFileName(filePath);
	for (var i=0; i<fileName.length; ++i)
	{
		var c = fileName[i];
		if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' )
			newFileName += c;
	}
	
	var dot = newFileName.lastIndexOf('.');
	if (!newFileName || dot == 0)
	{
		if (params["替换后为空名仍然进行"])
			newFileName = (Math.random()*100000 + Math.random()*100000).toString(10) + newFileName;
		else
			return "";
	}
	
	var newFilePath = RN.GetPath(filePath) + "\\" + newFileName;
	
	return newFilePath;
});