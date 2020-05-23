# PVZ小工具
  * 本人在aarch64/arm的Android 4.4.4、7.12、8.1、9.0中测试通过

  * 基于PvZ北美版6.1.1修改,基本的功能与6.1.1版一致

    ![app_info](doc/images/app_info.jpg)
    ![app_info_XI](doc/images/app_info_XI.jpg)
    ![pvz](doc/images/pvz.png)

  * 效果

    ![effect_0](doc/images/effect_0.png)

    ![effect_1](doc/images/effect_1.png)
## 支持什么功能?
  |功能|功能|
  |:-----|:-----|
  |0.设置阳光|1.设置金币|
  |2.开关免费植物|3.只出十只梯僵|
  |4.只出十只巨人|5.阵型压力测试|
  |6.自定义出怪列表|7.通过本关|
  |8.设置旗数|9.设置模式|
  |10.设置场景|11.设置卡槽|
  |12.设置冒险关卡|13.开关自动拾取|
  |14.解锁各游戏模式|15.移动场上梯子僵尸|
  |16.触发推车|17.黄油糊脸|
  |18.开关额外无尽入口|19.开关食人花秒吞|
  |20.开关额外特性|21.打印场上调试信息|
  |22.输出调试信息|
## 对游戏本体的修改之处
  * 此版本有五个无尽(购买了第一个游戏礼包后可以从Mini Games里进入)

  * 应用名是[植物大战僵尸北美修改版]/[植物大战僵尸北美XI版]

  * 无需手动放置数据包
## 相关功能的使用文档
  [部分相关代码](doc/code.txt)
## 在百度贴吧的发布帖
  [【重制】PvZ北美版修改器](https://tieba.baidu.com/p/6111235972)
## 相关安装教程
  [存档的处理教程](http://lonelystar.org/ResDownload/1_Android_Na.htm)

  感谢植物大战僵尸吧@囧丫乙 提供的资源

## 使用步骤
  * 1.**下载植物大战僵尸、修改器**

    链接: https://pan.baidu.com/s/1kWUH3Kj 密码: 4hby

    下载链接中的PVZ_CHEATER_{更新日期}-{COMMIT_HASH}.zip解压后一并安装里边两个文件就可以了

    |文件名|描述|
    |:----:|:----:|
    |PVZ_CHEATER_${更新日期}-aligned-debugSigned.apk|修改器程序|
    |PVZ_NA_XI_${更新日期}-aligned-debugSigned.apk|可用修改器,默认开启额外特性|
    |PVZ_NA_${更新日期}-aligned-debugSigned.apk|可用修改器,但是不默认开启额外特性的版本|

    **每次更新时都需要一并安装对应的游戏本体**

    **如果您已有原北美版,请备份好存档后卸载再安装本修改版本,然后恢复存档即可**

    如果您使用了正确的主程序,则应该类似于

    ![cheater_suceess](doc/images/cheater_success.png)

    如果您没有使用正确的主程序或者没有先打开PvZ,则

    ![cheater_wrong](doc/images/cheater_wrong.png)

    如果您使用的主程序版本与修改器版本不匹配,则

    ![cheater_mismatch](doc/images/cheater_mismatch.png)

## XI相关
  见![必读](doc/使用必读.txt)
  见![更新日志](更新日志.txt)

## THANKS
  tools/uber-apk-signer-1.0.0.jar @ https://github.com/patrickfav/uber-apk-signer

  JackpalTerminal @ https://github.com/jackpal/Android-Terminal-Emulator
