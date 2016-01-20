#ifndef _SysAdvEXTERNHDL_H_
#define _SysAdvEXTERNHDL_H_

#include <BaseExternHdl.hxx>

class SysAdvExternHdl : public BaseExternHdl
{
  public:
    SysAdvExternHdl(BaseExternHdl *nextHdl, PVSSulong funcCount, FunctionListRec fnList[])
      : BaseExternHdl(nextHdl, funcCount, fnList) {}

    virtual const Variable *execute(ExecuteParamRec &param);
};

#endif
