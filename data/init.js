
/*
var str='这是一个测试的字符串：{0} {1}'.format('Hello','world');
var str='这是一个测试的字符串：{0} {1}'.format(['Hello','world']); 或:
var str='这是一个测试的字符串：{str0} {str1}'.format({str0:'Hello',str1:'world'});
输出：这是一个测试的字符串：Hello world"
 */
String.prototype.format = function(args) {
	var result = this;
	if (arguments.length > 0) 
	{
		if ( arguments.length == 1 && typeof (args) == "object" && !(args instanceof Array) ) 
		{
			for (var key in args) 
			{
				if(args[key]!=undefined)
				{
					var reg = new RegExp("({" + key + "})", "g");
					result = result.replace(reg, args[key]);
				}
			}
		}
		else if ( !(args instanceof Array) )
		{
			for (var i = 0; i < arguments.length; i++) 
			{
				if (arguments[i] != undefined) 
				{
					var reg= new RegExp("({)" + i + "(})", "g");
					result = result.replace(reg, arguments[i]);
				}
			}
		}
		else //if (args instanceof Array)
		{
			for (var i = 0; i < args.length; i++) 
			{
				if (args[i] != undefined) 
				{
					var reg= new RegExp("({)" + i + "(})", "g");
					result = result.replace(reg, args[i]);
				}
			}
		}
	}
	return result;
}

String.prototype.ReplaceG = function(sFrom, sTo) {
	var result = this;
	
	sFrom = sFrom
		.replace(/[\\]/g,  '\\\\')
		.replace(/[\*]/g,  '\\*')
		.replace(/[\.]/g,  '\\.')
		.replace(/[\?]/g,  '\\?')
		.replace(/[\+]/g,  '\\+')
		.replace(/[\$]/g,  '\\$')
		.replace(/[\^]/g,  '\\^')
		.replace(/[\[]/g,  '\\[')
		.replace(/[\]]/g,  '\\]')
		.replace(/[\(]/g,  '\\(')
		.replace(/[\)]/g,  '\\)')
		.replace(/[\{]/g,  '\\{')
		.replace(/[\}]/g,  '\\}')
		.replace(/[\|]/g,  '\\|')
		.replace(/[\/]/g,  '\\/');
		
	var reg = new RegExp(sFrom, "g");
	result = result.replace(reg, sTo);
	
	return result;
}

RN.str = function(o)
{
	return JSON.stringify(o);
}

//跟据文件路径获得路径
RN.GetPath = function(filePath)
{
	if (filePath[filePath.length-1] == '\\' || filePath[filePath.length-1] == '/')
		filePath = filePath.substr(0, filePath.length-1);
	
	var split1 = filePath.lastIndexOf('/');
	var split2 = filePath.lastIndexOf('\\');
	var split = Math.max(split1, split2);
	if (-1 == split)
		return "";
	
	return filePath.substr(0, split);
}

//跟据文件路径获得文件名或文件夹名
RN.GetFileName = function(filePath)
{
	var split1 = filePath.lastIndexOf('/');
	var split2 = filePath.lastIndexOf('\\');
	var split = Math.max(split1, split2);
	if (-1 == split)
		return "";
	
	var newFileName = filePath.substr(split+1);
	
	return newFileName;
}

//跟据文件路径获得文件名或文件夹名（去除扩展名）
RN.GetFileNameWithoutEx = function(filePath)
{
	var fileName = RN.GetFileName(filePath);
	var dot = fileName.lastIndexOf('.');
	
	if (dot == 0)
		return "";
	else if (dot < 0)
		return fileName;
	else 
	{
		return fileName.substr(0, dot);
	}
	
	return "";
}

//跟据文件路径获得扩展名
RN.GetFileExtension = function(filePath)
{
	var fileName = RN.GetFileName(filePath);
	var dot = fileName.lastIndexOf('.');

	if (dot < 0)
		return "";
	else 
	{
		return fileName.substr(dot + 1);
	}
	
	return "";
}

//终结本次运行
RN.Exit = function()
{
	RN.TERMINATED = true;
}
