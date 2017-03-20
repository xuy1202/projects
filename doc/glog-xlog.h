
 ./configure --prefix=/home/dev --enable-namespace=xlog

 
 src/glog/logging.h, change "google" to "xlog"
  605     std::ostream& out, const xlog::DummyClassToDefineOperator&) {
