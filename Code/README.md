\## 项目简介

这是一个 51 单片机学习项目，全部模块


\##开发软件及烧录程序

开发所需的软件已备份在网盘中（避免上传大文件到 Git）：

* 通过网盘分享的文件：c51项目
* 链接: https://pan.baidu.com/s/1X4ad8DISc6k9mJHP5UbO8A 提取码: 0721



\##git的常用命令（对于初学者）

```linux
git config --global user.name "min-lfl"  设置全局git用户名

git config --global user.email "2775381313@qq.com"  设置全局git邮箱

git add . 把本地文件加入暂存区，告诉git我想存储哪些文件（或者说为哪些文件创建版本）

git commit -m "<版本描述>"  保存一次版本，和git add .配合使用

git log 查看历史版本以及版本描述，加--stat可以查看每次修改了哪些文件

git diff <版本id> 查看某个版本具体修改了哪些内容，版本id通过git log查看，在commit后面     一般格式是f1a9165b83b4da3993f98e9f9d44e4d7dab89a7d

git reset --hard <版本id> 可以回退到某个版本，第一种回退方法

git checkout <版本id>  可以回退到某个版本，第二种回退方法

git branch 查看分支

git checkout -b develop 创建一个名为develop的分支



———————————第一次连接创建方法—————————————

git remote add <链接别名相当于域名作用>  <网站上复制的ssh链接>

git remote -v 查看有没有添加成功

ssh-keygen -t rsa -b 4096 -C "2775381313@qq.com" -f /c/Users/廖芳淋/.ssh/id\\\_rsa 生成密钥

eval "$(ssh-agent -s)"  启动SSH agent

ssh-add /c/Users/Administrator/.ssh/id\\\_rsa   添加密钥

cat /c/Users/廖芳淋/.ssh/id\\\_rsa.pub 复制公钥内容

然后再github网页设置Settings里的SSH and GPG keys里的New SSH key里添加上条复制的密钥

git add . 打包所有项目文件

git commit -m "添加注释"  创建新的版本

git push <上面链接别名> master  同步本地代码到远程仓库master分支

git pull  <上面链接别名> master  拉取所有的云端项目到本地



————————————————————————————————

git clone <代码仓库地址>绑定远程github仓库

git remote add origin <代码仓库地址>和上面共同执行，绑定远程github仓库

git remote remove origin 删除绑定链接

这三条是用https协议的方法，由于不稳定已经弃用，用上面提到的ssh协议就好

————————————————————————————————
# 2. 从 "干净" 的提交创建一个新分支
git branch feature_v2 c1efaa2

# 3. 切换到新分支开始重新实现
git checkout feature_v2
# ... 重新编写代码 ...

# 4. 提交新的实现
git add .
git commit -m "重新实现功能，这次更好"

# 5. 主分支 (master) 保持不变，仍然包含 e315432
git checkout master
git log


```

\##作者

廖芳淋

