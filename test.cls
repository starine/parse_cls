有#开头的行是数据行，其他是注释。
每行开头和结尾都可能有空格/tab。

DEFINE定义了若干常量，等号前后可能有空格
#DEFINE IDLength=10 
#DEFINE NameLength=20 

OBJECT定义了一个对象
#OBJECT=objA 
    ELEMENT定义一个属性，TYPE是一个字母加上可选的特殊标记Key（可以认为中间没有空格）
    LENGTH可以是数字或者用$引用前面的常量
	#ELEMENT=szName      	TYPE=C       	LENGTH=$NameLength 
	#ELEMENT=szRdfID      	TYPE=C,Key     	LENGTH=$IDLength 
	#ELEMENT=nID      		TYPE=I       	LENGTH=2
#END OBJECT
END语句用于关闭对象定义

#OBJECT=objB
#ELEMENT=szName      	TYPE=C       	LENGTH=1
#END OBJECT