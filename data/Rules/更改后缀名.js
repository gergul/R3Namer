RN.PARAMS.push({
	"查找目标后缀名" 				: "",
	"查找目标后缀名_DESC"			: "将要被替换的后缀名，不带“.”",
		
	"替换成的后缀名"				: "",
	"替换成的后缀名_DESC"			: "替换到的后缀名，不带“.”"
});

RN.RULES.push(function(filePath, isFile, params)
{
	if (!isFile)
		return "";
	
	if (params["查找目标后缀名"].toUpperCase() == params["替换成的后缀名"].toUpperCase())
	{
		print("“查找目标后缀名”与“替换成的后缀名”不能相同。\n");
		RN.Exit();
		return ;
	}
	
	var ex = RN.GetFileExtension(filePath).toUpperCase();
	if (ex != params["查找目标后缀名"].toUpperCase())
		return "";
	
	var fileName = RN.GetFileNameWithoutEx(filePath);
	var newFileName;
	if (params["替换成的后缀名"] != "")
		newFileName = fileName + "." + params["替换成的后缀名"];
	else
		newFileName = fileName;

	var newFilePath = RN.GetDir(filePath) + newFileName;
	
	return newFilePath;
});