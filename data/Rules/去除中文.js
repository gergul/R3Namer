RN.PARAMS.push({
	"替换后为空名仍然进行"			: true, 
	"替换后为空名仍然进行_DESC"		: "如果替换后文件名为空时，是否进行重命名（文件名为随机数）。",
	
	"目标类型"						: 0,
	"目标类型_DESC"					: "目标类型：0->同时应用文件和文件夹（默认）；1->只应用文件；2->只应用文件夹"
});

RN.RULES.push(function(filePath, isFile, params)
{
	if ( (params["目标类型"] != 0 && ((params["目标类型"]==1&& !isFile) || (params["目标类型"]==2&& isFile))) ||
		 (params["目标类型"] != 0 && params["目标类型"] != 1 && params["目标类型"] != 2) )
		return "";
	
	var newFileName = "";
	var fileName = RN.GetFileName(filePath);
	
	/*var sEscape = escape(fileName);
	var ui = sEscape.indexOf('%u');
	while (ui > -1)
	{
		sEscape = sEscape.substr(0, ui) + sEscape.substr(ui+6);
		ui = sEscape.indexOf('%u');
	}
	newFileName = unescape(sEscape);*/
	
	var reg = new RegExp("[\\u4E00-\\u9FFF]+" , "g");
	newFileName = fileName.replace(reg, '');
	
	var dot = newFileName.lastIndexOf('.');
	if (!newFileName || dot == 0)
	{
		if (params["替换后为空名仍然进行"])
			newFileName = (Math.random()*100000 + Math.random()*100000).toString(10) + newFileName;
		else
			return "";
	}
	
	var newFilePath = RN.GetDir(filePath) + newFileName;
	
	return newFilePath;
});