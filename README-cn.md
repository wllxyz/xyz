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


