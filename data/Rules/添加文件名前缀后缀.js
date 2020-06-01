RN.PARAMS.push({
	"添加文件名前缀" 				: "",
	"添加文件名前缀_DESC"			: "例如：填入文件名前缀：“铜锣湾-”，则“顶层平面图.pdf”这个文件名将变为“铜锣湾-顶层平面图.pdf”",
		
	"添加文件名后缀" 				: "",
	"添加文件名后缀_DESC"			: "例如：填入文件名后缀：“-J7”，则“顶层平面图.pdf”这个文件名将变为“顶层平面图-J7.pdf”",
});

RN.RULES.push(function(filePath, isFile, params)
{
	if (!isFile)
		return "";
	
	if (params["添加文件名前缀"].toUpperCase() == "" && params["添加文件名后缀"].toUpperCase() == "")
	{
		print("“添加文件名前缀” 和 “添加文件名后缀”不能同时为空。\n");
		RN.Exit();
		return ;
	}
	
	var ex = RN.GetFileExtension(filePath);
	if (ex != "")
		ex = '.' + ex;
	
	var fileName = RN.GetFileNameWithoutEx(filePath);
	var newFileName = fileName;
	if (params["添加文件名前缀"] != "")
		newFileName = params["添加文件名前缀"] + newFileName;
	
	if (params["添加文件名后缀"] != "")
		newFileName = newFileName + params["添加文件名后缀"];

	var newFilePath = RN.GetDir(filePath) + newFileName + ex;
	
	print(newFilePath + '\n');
	
	return newFilePath;
});