xyz是为人工智能和编译目标设计的脚本语言和工具,结合了lisp和yacc的一些特性.
=======================================================================

#1. 如何获取xyz?

	git clone https://github.com/wllxyz/xyz.git

#2. 怎样编译xyz?

	cd xyz/xyz
	make

#3. 怎样安装xyz?

安装完成后:  

	make install install_dir=<你想要安装的路径>  

把xyz的安装路径添加到PATH变量中去

#4. 怎样运行xyz?

最普通的应用场景,运行:  

	wll <grammar-file> <input-file>

#5. 如何使用xyz?

如果你只想了解xyz语言长什么样?能干嘛?那么你可以通过简单的使用示例来学习如何编写xyz脚本来完成你的工作.  
xyz语言的文法很简单,如果你熟悉文法范式,最核心的xyz自解释文法不过十几条而已.  

以下是WLL1解析器能够识别的xyz文法定义(核心语法):  
	
	<start>--><wll0>==><start>-->$LEFT_QUOTE$LOAD_TRANSLATIONS$SEPERATOR$IGNORE$LEFT_QUOTE<wll0>$RIGHT_QUOTE$RIGHT_QUOTE;
	<wll0>--><translation>";\n"==><wll0>--><translation>";\n";
	<wll0>--><translation>";\n"<wll0>==><wll0>--><translation>";\n"<wll0>;
	<translation>--><source_rule>"==>"<destination_rule>==><translation>--><source_rule>"==>"<destination_rule>;
	<source_rule>--><rule>==><source_rule>--><rule>;
	<destination_rule>--><rule>==><destination_rule>--><rule>;
	<rule>--><root>"-->"<expression>==><rule>--><root>"-->"<expression>;
	<root>--><variable>==><root>--><variable>;
	<expression>--><symbol>==><expression>--><symbol>;
	<expression>--><symbol><expression>==><expression>--><symbol><expression>;
	<symbol>--><variable>==><symbol>--><variable>;
	<symbol>--><constant>==><symbol>--><constant>;
	<symbol>--><remark>==><symbol>--><remark>;
	<variable>-->"<"<identifier>">"==><variable>-->"<"<identifier>">";
	<constant>-->'"'<string>'"'==><constant>-->'"'<string>'"';
	<constant>-->"'"<original_string>"'"==><constant>-->"'"<original_string>"'";
	<remark>-->"$"<identifier>==><remark>-->"$"<identifier>;

下面我把它简单的翻译成白话文:  
**xyz语言程序**是由一系列文法翻译规则构成,每一个文法翻译规则都以;和换行结束.  
每一个**文法翻译规则**都是由源语言规则和目标语言规则两部分构成,中间用==>符号表示源语言规则到目标语言规则的对应转化.  
**源语言规则**和**目标语言规则**都以文法产生式的形式表示.  
每一个**文法产生式**都是由一个文法变量和对应的文法表达式两部分构成,中间用-->符号表示文法变量到文法表达式的展开定义.  
**文法表达式**由一系列的文法符号构成.  
**文法符号**有三种:文法变量,文法常量和文法标记.  
**文法变量**是由<>括起来的标识符.  
**文法常量**是由""或者''引起来的字符串.  
__文法标记__是由$符号开头的标识符.  

下面我举例说明:  
文法常量:   
	
	双引号引起来的文法常量:"abc","adf@#42349","\""(表示一个双引号"),"\n"(表示一个换行符),"\\"(表示倒斜杆\)  
	单引号引起来的文法常量:'yqueyr2838"09090!@#$$%%^&' 单引号引起来的字符串可以包含除'本身以外的所有其他可打印字符(目前没有支持中文字符)  

文法变量:  
	
	如<a>, <b>, <_abc>, <_123abc>, <_12>, <x1>都是合法的文法变量.  

文法标记:  
	
	如$REMARK, $WLL0, $ADD, $LEFT_QUOTE, $IGNORE 等等, 文法标记一般有特殊含义的,WLL的解释器会对文法标记做特殊处理.  
	如果我们不用写自解释扩展文法或者是文法解释器,我们可以暂时不用关心文法标记.  

文法表达式:  
	
	如<symbol><expression>,这是由两个文法变量构成的文法表达式.  
	如"abc",这是由一个文法常量构成的文法表达式.  
	如'"'<string>'"',这是由文法常量和文法变量一起构成的文法表达式.  
	如"",这是一个特殊的空字符串文法常量构成的文法表达式.  

文法产生式:  
	
	如<remark>-->"$"<identifier>,表示<remark>文法变量可以被扩展为由"$"和<identifier>构成的文法表达式.  
	如<expression>--><symbol><expression>,表示<expression>文法变量可以递归地定义为由一个<symbol>文法变量和另一个<expression>文法变量构成.  

文法翻译规则:  
	
	如<start>--><wll0>==><start>-->$LEFT_QUOTE$LOAD_TRANSLATIONS$SEPERATOR$IGNORE$LEFT_QUOTE<wll0>$RIGHT_QUOTE$RIGHT_QUOTE;  
	表示当源语言(输入)识别了<start>--><wll0>这条文法产生式后自动生成相应的目标语言的文法产生式结构  
	<start>-->$LEFT_QUOTE$LOAD_TRANSLATIONS$SEPERATOR$IGNORE$LEFT_QUOTE<wll0>$RIGHT_QUOTE$RIGHT_QUOTE,  
	而这条目标语言的文法产生式的文法表达式是由标记语言构成的s-表达式,经过WLL解释器执行后会调用WLL文法加载器加载转换后的<wll0>文法变量.  

	如<e>--><e>"+"<t>==><e>--><e><t>"+";  
	表示将中缀表达式的加法表达式转换为后缀表达式形式.  

最新的xyz完整自解释文法请参考[xyz/data/grammar](xyz/data/wll1/grammar.wll1-TopDown)  
补充xyz文法说明：   

        1. #开头行为注释行  
        2. 允许空行；除了文法表达式的文法符号之间不能有空格，其他符号之间都可以有空格。  
        3. 引入s-表达式，其形式和lisp的s表达式相同: (函数 参数1 ... 参数N) 函数（第一个参数）部分是remark标记，目前支持的remark标记有：  

		支持S表达式求解(WLL1文法自解释)	
		$IGNORE;			//忽略$IGNORE$LEFT_QUOTE...$RIGHT_QUOTE括号内的符号的求值
		$EVAL;				//执行求值动作
		$LEFT_QUOTE;		//S表达式的左括号,用来界定前缀表达式
		$RIGHT_QUOTE;		//S表达式的右括号,用来界定前缀表达式
		$SEPERATOR;			//S表达式的参数分隔符号,用来分隔前缀表达式参数
		$LOAD_TRANSLATIONS;	//调用Wll0Loader加载文法规则
		$ADD_TRANSLATIONS;	//调用Wll0Loader加载文法规则,和LOAD_TRANSLATIONS的区别是ADD_TRANSLATIONS不会清空之前的文法规则
		支持运算控制
		$ADD;				//整数加,1..n个参数
		$SUB;				//整数减,1..n个参数
		$MUL;				//整数乘,1..n个参数
		$DIV;				//整数除,1..n个参数
		$TRUE;				//逻辑真
		$FALSE;				//逻辑假
		$COND;				//条件控制
		$LOOP;				//循环控制
		$EQ;				//等于
		$LT;				//小于(字典序)
		$AND;				//与
		$OR;				//或
		$NOT;				//非
		$SHELL;				//执行shell命令
		支持变量存储
		$DEF;				//变量定义
		$SET;				//变量赋值,如果变量之前没有定义则自动定义
		$GET;				//变量引用,如果变量没有定义,返回空值
		$PUSH_DATA;			//初始化变量空间并压栈
		$POP_DATA;			//销毁本层变量空间并出栈
		$PUSH;				//表达式值压入参数栈顶
		$POP;				//参数栈顶值出栈到变量
	
#6. wll和xyz的关系?
	
wll是xyz语言的编译和解释器,一个是指语言编译工具,另一个是指语言的文法或源程序.  
当初之所以取wll和xyz的名字,是因为它是我自己和另一个精神上的支持者的名字的缩写.  
后来逐渐确定为xyz有另外一层含义,因为最开始我的初衷是写一个编译器的编译器,因此它应该能够编译任何语言,成为任何一门计算机语言的编译器.  

#7. 如何加入xyz项目的开发?
	
如果你有兴趣和时间精力投入xyz项目的开发,我会欢迎你的加入.因为xyz是一个开放的项目.  
你可以自由的在研究和商业项目中使用xyz项目的源码,不附加任何条件.  
我也希望你能够反馈更多更好的创意到开源项目中来,共同推动开源运动的发展.  
因为我相信最好的技术都是免费的,为全人类的共同发展服务的,虽然包括我自己在内的大多数优秀的程序员还在为谋生而编写私有的商业代码.  

感兴趣的加入者可以通过git平台或者[我的邮箱](mailto://wll_xyz@163.com)和QQ46760694同我联系.  

#8. 跋
从最初2006年在川大读研究生时为编译实验课程写完pl0编译器后,有一个"写编译器的编译器"的想法,到现在我的儿子刚刚满一周岁,十年已经过去了.  
十年磨一剑,虽然自己还不是特别满意,但是比起离开百度时完成的雏形,这已经算是一个成品了.		
再不放出来,怕是要忘却了.就算是当做一个纪念吧.  
纪念这过去的十年青春岁月,纪念我的儿子的周岁,纪念即将到来的川大周年庆典.  

