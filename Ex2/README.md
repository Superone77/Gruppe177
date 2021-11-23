macOS下无法使用gcc编译带有openMP语句的问题的解决办法：

使用如下语句编译：
```
brew install libomp
gcc/clang -Xpreprocessor -fopenmp -lomp file.cxx
```
然后运行：
```
./a.out
```
