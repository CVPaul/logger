## 基本信息
- 创建时间：2020-04-08
- 项目内容：基于Boost构建的一个LOG库
- 项目出发点：C++也有一些日志库（如：`GLOG`, `Log4j`的C++版本,...），Boost本身也支持日志，但是`GLOG`要额外配置，`Log4j`原本是Java的，也需要额外配置，Boost自带的日志接口有些用户门槛，所以这里对其进行了封装，实现了如下功能

## 功能点一：同步和异步无缝切换

- 通过`logging.h`文件夹中的`USING_ASYNC_LOG_MODE`作为切换的开关
- 同步的好处：日志不会丢
- 异步的好处：速度快但是可能会丢日志（之前的测试为：同步约6.3ms -> 异步0.3ms）

## 功能点二：直接logger_xxx调用不同level的日志打印宏

- 经过反复的探索，发现还是宏输出的方式最适合C++: `LOGGER_XXX << "LOGGER INFO ...."`
    - `GLOG`和`BOOST`本身也使用可这种方式
- 宏输出也方便了`__LINE__`,`__FILE__`,`__FUNCTION__`信息的打印，方便定位问题

## 功能点三：实现了日志切割逻辑

- 利用`BOOST`的`logging::sinks::text_multifile_backend`的特点实现了日志自动切割

## 其它

- 编译的环境是`VS Code` + `CMake`来实现跨平台的；参考[这里](https://zhuanlan.zhihu.com/p/127046795)
- `VS Code`中`.vscde`下的`launch.json`和`tasks.json`实现了编译器可选
- `CMake`实现了跨平台编译
- `Boost`的安装：相较于其它的开源库的编译和安装`Boost`已经是非常的简单了，网上也有很多资料，查查就会了，例如：[这里](https://www.jianshu.com/p/9b7f68ab0f89)

## 使用说明
- `git clone`代码
- `VScode`按照[这里](https://zhuanlan.zhihu.com/p/127046795)配置好
- 安装上`Boost`
- VSCode中按`F5`按照提示一路next