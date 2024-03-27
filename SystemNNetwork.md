## 设计模式七大原则
1) 单一职责原则：一个类应该只负责一项职责。
2) 接口隔离原则：客户端不应该依赖它不需要的接口
3) 依赖倒转(倒置)原则：程序要依赖于抽象接口，不要依赖于具体实现
4) 里氏替换原则：父类中凡是已经实现好的方法，实际上是在设定规范和契约
5) 开闭原则：模块和函数应该 对扩展开放( 对提供方)，对 修改关闭( 对使用方)。
6) 迪米特法则：一个对象应该对其他对象保持最少的了解
7) 合成复用原则：尽量使用合成/聚合的方式，而不是使用继承

## windows消息系统由哪几部分构成
消息队列，消息循环，消息处理回调函数

windows是一个消息驱动的系统，会有一个总的系统消息的队列，鼠标、键盘等等都会流入到这个队列中，同时会为每个线程维护一个消息队列（注意默认是有GUI调用的线程才有，对于没有GUI或者窗口的线程，只有当在线程内调用get/peek message 才会自动创建一个消息队列），线程是容纳消息队列的基本单元，系统会把属于不同线程的消息投递到属于线程的消息队列中

当线程调用get/peek message时会从系统的消息队列中取出一个本线程内的消息。（get方法是阻塞的会等到一个消息拿出来为止，取到后会从队列中移除，而peek方法只是快速的查看一下，有取出没有返回，并且可以选择取到后是否从队列中移除。）

对于有窗口的线程，在取出message后通常调用dispatchmessage将其推送给这个线程的窗口过程函数WndProc上，通常我们在WndProc函数里面响应不同窗口对消息的处理，WndProc是我们在创建窗口时必须指定的。而没有窗口的线程，默认没有消息队列，但是你调用Get/Peek message会自动建立一个，并得到这个线程的消息。

线程间消息传递

通常线程将消息传递是进程间通信最重要的方式，线程间消息传递有两种API：

PostThreadMessage ：直接发送给目标线程 这是对于那些不是发给某个窗口的消息

PostMessage：发送给某个线程下的某个窗口

## 什么是消息映射
“消息映射”用于指定哪些函数将处理特定类的各种消息。

## 报文分片和TCP重组
一个IP数据报则可能会有8192字节，超过以太帧的最大限制，那么这时就需要IP分片，分批进行传输。

发送方会在IP层将要发送的数据分成多个数据包分批发送，而接收方则将数据按照顺序再从新组织起来，等接收到一个完整的数据报之后，然后再提交给上一层传输层。

TCP协议可以避免了IP分片的发生，它会在TCP层对数据进行处理，对数据进行分段，IP分片用的多的在UDP协议
## 面向流和面向数据报
 TCP 是面向字节流的协议，UDP 是面向报文的协议，是因为操作系统对 TCP 和 UDP 协议的发送方的机制不同，也就是问题原因在发送方。

> 为什么 UDP 是面向报文的协议？\
操作系统不会对消息进行拆分，在组装好 UDP 头部后就交给网络层来处理，所以发出去的 UDP 报文中的数据部分就是完整的用户消息，也就是每个 UDP 报文就是一个用户消息的边界，这样接收方在接收到 UDP 报文后，读一个 UDP 报文就能读取到完整的用户消息。\
操作系统在收到 UDP 报文后，会将其插入到队列里，队列里的每一个元素就是一个 UDP 报文，这样当用户调用 recvfrom() 系统调用读数据的时候，就会从队列里取出一个数据，然后从内核里拷贝给用户缓冲区。

> 为什么 TCP 是面向字节流的协议？\
当用户消息通过 TCP 协议传输时，消息可能会被操作系统分组成多个的 TCP 报文，也就是一个完整的用户消息被拆分成多个 TCP 报文进行传输。\
这时，接收方的程序如果不知道发送方发送的消息的长度，也就是不知道消息的边界时，是无法读出一个有效的用户消息的，因为用户消息被拆分成多个 TCP 报文后，并不能像 UDP 那样，一个 UDP 报文就能代表一个完整的用户消息。\
我们不能认为一个用户消息对应一个 TCP 报文，正因为这样，所以 TCP 是面向字节流的协议。

## 粘包会有什么问题，如何解决
粘包的问题出现是因为不知道一个用户消息的边界在哪，如果知道了边界在哪，接收方就可以通过边界来划分出有效的用户消息。
1. 固定长度的消息；
2. 特殊字符作为边界；
3. 自定义消息结构。

## 三次握手四次挥手
1. 客户端发送一个带有SYN标志的TCP报文 -- 客户端什么都不能确认；服务器确认了对方发送正常，自己接收正常
2. 服务器发送一个带有SYN和ACK标志的报文 -- 客户端确认了：自己发送、接收正常，对方发送、接收正常；服务器确认了：对方发送正常，自己接收正常
3. 客户端再次发送一个带有ACK标志的报文（可以携带数据） -- 双方确认：自己发送、接收正常，对方发送、接收正常；

1. 客户端发送一个FIN包，用来关闭客户端到服务器的数据传送。-- 客户端 established -> fin_wait1
2. 服务器收到这个FIN包后，发送一个ACK确认包，告知已经接受到客户端的关闭请求，但还没有准备好关闭连接。客户端 fin_wait1 -> fin_wait2 服务端 established -> close_wait
3. 服务器准备好关闭连接时，向客户端发送自己的FIN包，请求关闭从服务器到客户端的数据传输。客户端 fin_wait2 -> time_wait  服务端 close_wait -> last_ack
4. 客户端收到服务器的FIN包后，发送一个ACK包作为应答，并会进入TIME_WAIT状态等待可能需要重传的ACK包 客户端 time_wait 服务端 close_wait -> closed

> 如果第三次握手丢失了，客户端服务端会如何处理？\
服务器发送完 SYN-ACK 包，如果未收到客户端响应的确认包，也即第三次握手丢失。那么服务器就会进行首次重传，若等待一段时间仍未收到客户确认包，就进行第二次重传。如果重传次数超过系统规定的最大重传次数，则系统将该连接信息从半连接队列中删除。\
注意，每次重传等待的时间不一定相同，一般会是指数增长，例如间隔时间为 1s，2s，4s，8s

> 客户端收到 SYN+ACK 返回的 ACK 包丢失\
当客户端在 ESTABLISHED 状态下，开始发送数据包时，会携带上一个 ACK 的确认序号，所以哪怕客户端响应的 ACK 包丢了，服务端在收到这个数据包时，能够通过包内 ACK 的确认序号，正常进入 ESTABLISHED 状态。

> 如果第三次挥手前，客户端收到 ACK 后，服务端跑路了？\
客户端在收到「ACK」后，进入了 FIN-WAIT-2 状态，等待服务端发来的「FIN」包，而如果服务端跑路了，这个包永远都等不到。
在 TCP 协议中，是没有对这个状态的处理机制的。但是协议不管，系统来凑，操作系统会接管这个状态，例如在 Linux 下，就可以通过 tcp_fin_timeout 参数，来对这个状态设定一个超时时间。

> 客户端断开连接的 FIN 包丢失\
  这种情况下，会触发超时重传机制，不会去考虑是自己发出的包丢失，还是无法收到对方发来的恢复，直到关闭连接。

> 服务端第一次返回的 ACK 包丢失\
  这种情况下，客户端无法收到服务端返回的 ACK 包，会触发重传机制，重传 FIN 包。
  而服务器收到重传的 FIN 包后，会立即在重传对 FIN 包的 ACK 包。
  而此时服务器已经进入 CLOSED-WAIT 状态，开始做断开连接前的准备工作。当准备好之后，会回复 FIN+ACK，这个消息是携带了之前 ACK 的响应序号的，这就是第三次挥手的包。

> 服务器发送的 FIN+ACK 包丢失\
  这是第三次挥手的包丢失了，客户端处于 FIN-WAIT-2 状态 (之前的 ACK 收到)，会一直等待到超时\
  而服务端则收不到对应的 ACK 确认包，则会进行超时重传。

> 客户端最后返回的 ACK 包丢失\
  客户端在回复 ACK 后，会进入 TIME-WAIT 状态，开始长达 2MSL 的等待，服务端因为没有收到 ACK 的回复，会重试一段时间，直到服务端重试超时后主动断开。

## sendmessage vs postmessage 
SendMessage waits for the target window to process the message, while PostMessage places the message in the message queue and returns immediately

## 系统会自动打开和关闭的三个标准文件是
操作系统会默认打开三个标准输入输出流:标准输入，标准输出，标准错误。

## strdup() 函数
```
char *strdup(char *str)；
```
将字符串复制到新建立的空间,该函数会先用malloc()配置与参数str字符串相同的空间大小，然后将参数str字符串的内 容复制到该内存地址，然后把该地址返回。

## Windows bat脚本命令大全

## Linux Shell 脚本命令大全
1. top：查看内存/显示系统当前进程信息
2. df -h：查看磁盘储存状况
3. iotop：查看IO读写（yum install iotop安装）
4. iotop -o：直接查看比较高的磁盘读写程序
5. netstat -tunlp | grep 端口号：查看端口号占用情况（1）
6. lsof:列出当前系统打开文件
7. uptime：查看报告系统运行时长及平均负载
8. ps aux：查看进程

基础
1、查看目录与文件：ls
ls -la：显示当前目录下所有文件的详细信息
2、切换目录：cd
cd /home 进入 ‘/ home’ 目录
cd … 返回上一级目录
cd …/… 返回上两级目录
3、显示当前目录：pwd
pwd
4、创建空文件：touch
touch desc.txt：在当前目录下创建文件desc.txt
5、创建目录：mkdir
mkdir test：在当前目录下创建test目录
mkdir -p /opt/test/img：在/opt/test目录下创建目录img，若无test目录，先创建test目录
6、查看文件内容：cat
cat desc.txt：查看desc.txt的内容
7、分页查看文件内容：more
more desc.txt：分页查看desc.txt的内容
8、查看文件尾内容：tail
tail -100 desc.txt：查看desc.txt的最后100行内容
9、拷贝：cp
cp desc.txt /mnt/：拷贝desc.txt到/mnt目录下
cp -r test /mnt/：拷贝test目录到/mnt目录下
10、剪切或改名：
mv desc.txt /mnt/：剪切文件desc.txt到目录/mnt下
mv 原名 新名
11、删除：rm
rm -rf test：删除test目录，-r递归删除，-f强制删除。危险操作，务必小心，切记！
12、搜索文件：find
find /opt -name ‘*.txt’：在opt目录下查找以.txt结尾的文件


13、显示或配置网络设备：ifconfig
ifconfig：显示网络设备情况
14、显示网络相关信息：netstat
netstat -a：列出所有端口
netstat -tunlp | grep 端口号：查看进程端口号

    15、显示进程状态：ps
ps -ef：显示当前所有进程
ps-ef | grep java：显示当前所有java相关进程
16、查看目录使用情况：du
du -h /opt/test：查看/opt/test目录的磁盘使用情况
17、查看磁盘空间使用情况：df
df -h：查看磁盘空间使用情况
18、显示系统当前进程信息：top
top：显示系统当前进程信息
19、杀死进程：kill
kill -s 9 27810：杀死进程号为27810的进程，强制终止，系统资源无法回收
20、压缩和解压：tar
tar -zcvf test.tar.gz ./test：打包test目录为test.tar.gz文件，-z表示用gzip压缩
tar -zxvf test.tar.gz：解压test.tar.gz文件
21、改变文件或目录的拥有者和组：chown
chown nginx:nginx desc.txt：变更文件desc.txt的拥有者为nginx，用户组为nginx
chown -R nginx:nginx test：变更test及目录下所有文件的拥有者为nginx，用户组为nginx
22、改变文件或目录的访问权限：chmod
chmod u+x test.sh：权限范围：u(拥有者)g(郡组)o(其它用户)， 权限代号：r(读权限/4)w(写权限/2)x(执行权限/1)#给文件拥有者增加test.sh的执行权限
chmod u+x -R test：给文件拥有者增加test目录及其下所有文件的执行权限
23、文本编辑：vim
vim三种模式：命令模式，插入模式，编辑模式。使用ESC或i或：来切换模式。
命令模式下:q退出 :q!强制退出 :wq!保存退出 :set number显示行号 /java在文档中查找java yy复制 p粘贴
vim desc.txt：编辑desc.txt文件
24、关机或重启：shutdown
shutdown -h now：立刻关机
shutdown -r -t 60：60秒后重启
shutdown -r now：重启(1)
reboot：重启(2)
25、帮助命令：man


1、文件管理
cat more less tail
chown	chgrp	chmod
cksum	cmp	diff
file	find	 ln
mv	mkdir patch	cp	rm scp ftp
slocate	split	tee	tmpwatch
touch	which	whereis
grep	join awk	sed
 
4、磁盘管理
cd	df	dirs	du
mkdir pwd
mount	umount rmdir
stat	tree		ls

badblocks	cfdisk	dd	e2fsck
ext2ed	fsck	fsck.minix	fsconf
fdformat	hdparm	mformat	mkbootdisk
mkdosfs	mke2fs	mkfs.ext2	mkfs.msdos
mkinitrd	mkisofs	mkswap	mpartition
swapon	symlinks	sync	mbadblocks
mkfs.minix	fsck.ext2	fdisk	losetup
mkfs	sfdisk	swapoff	 

6、网络通讯
apachectl	arpwatch	dip	getty
mingetty	uux	telnet	uulog
uustat	ppp-off	netconfig	nc
httpd	ifconfig	minicom	mesg
dnsconf	wall	netstat	ping
pppstats	samba	setserial	talk
traceroute	tty	newaliases	uuname
netconf	write	statserial	efax
pppsetup	tcpdump	ytalk	cu
smbd	testparm	smbclient	shapecfg

7、系统管理
adduser	chfn	useradd	date
exit	finger	fwhios	sleep
suspend	groupdel	groupmod	halt
kill	last	lastb	login
logname	logout	ps	nice
procinfo	top	pstree	reboot
rlogin	rsh	sliplogin	screen
shutdown	rwho	sudo	gitps
swatch	tload	logrotate	uname
chsh	userconf	userdel	usermod
vlock	who	whoami	whois
newgrp	renice	su	skill
w	id	groupadd	free

8、系统设置
reset	clear	alias	dircolors
aumix	bind	chroot	clock
crontab	declare	depmod	dmesg
enable	eval	export	pwunconv
grpconv	rpm	insmod	kbdconfig
lilo	liloconfig	lsmod	minfo
set	modprobe	ntsysv	mouseconfig
passwd	pwconv	rdate	resize
rmmod	grpunconv	modinfo	time
setup	sndconfig	setenv	setconsole
timeconfig	ulimit	unset	chkconfig
apmd	hwclock	mkkickstart	fbset
unalias	SVGATextMode	gpasswd	 

9、备份压缩
ar	bunzip2	bzip2	bzip2recover
gunzip	unarj	compress	cpio
dump	uuencode	gzexe	gzip
lha	restore	tar	uudecode
unzip	zip	zipinfo	 


## Linux internals

