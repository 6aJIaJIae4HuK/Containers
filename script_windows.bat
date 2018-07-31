md tmp
cd tmp
cmake -DBOOST_ROOT=D:\tmpBoost\boost_1_66_0 ..\
cmake --build . --config Release --target INSTALL
ctest -C Release -V -R test1
cd ..\
rmdir /S /Q tmp