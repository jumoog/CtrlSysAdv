#include <SysAdvExternHdl.hxx>
#include "TextVar.hxx"
#include "BitVar.hxx"
#include "DynVar.hxx"

#include <stdio.h>

//------------------------------------------------------------------------------

static FunctionListRec fnList[] =
{
  // TODO add for every new function an entry
  { INTEGER_VAR, "exec", "(string cmd, dyn_string &output, bool wantStdErr = false)", false }
};

CTRL_EXTENSION(SysAdvExternHdl, fnList)

//------------------------------------------------------------------------------

const Variable *SysAdvExternHdl::execute(ExecuteParamRec &param)
{
  enum
  {
    // TODO add here all your function numbers in the sequence used in the FunctionListRec array
    F_exec  = 0
  };

  // TODO for the different return types you need corresponding static Variables
  static IntegerVar integerVar;

  switch ( param.funcNum )
  {
    // ---------------------------------------------------------------------
    // TODO change to your function
    case F_exec:
    {
      param.thread->clearLastError();

      // check number of arguments
      // TODO
      if ( !param.args || (param.args->getNumberOfItems() < 2) )
      {
        ErrClass err(ErrClass::PRIO_WARNING,
                     ErrClass::ERR_CONTROL, ErrClass::ARG_MISSING,
                     param.thread->getLocation(), param.funcName);

        ErrHdl::error(err);
        param.thread->appendLastError(err);

        integerVar.setValue(-1); return &integerVar;
      }

      TextVar command;
      Variable *outputVar = 0;
      PVSSboolean wantStdErr = PVSS_FALSE;
      DynVar output(TEXT_VAR);

      command = *(param.args->getFirst()->evaluate(param.thread));
      outputVar = (param.args->getNext ()->getTarget(param.thread));

      if ( ! outputVar )
      {
        ErrClass err(ErrClass::PRIO_WARNING,
                     ErrClass::ERR_CONTROL, ErrClass::NO_LVALUE,
                     param.thread->getLocation(), param.funcName);

        ErrHdl::error(err);
        param.thread->appendLastError(err);

        integerVar.setValue(-1); return &integerVar;
      }

      if (param.args->getNumberOfItems() == 3)    // third parameter given
        wantStdErr = ((BitVar*)(param.args->getNext()->evaluate(param.thread)))->getValue();

      std::string cmdStr = command.getString().c_str();
      FILE *stream = 0;
      const int max_buffer = 16 * 1024;
      char buffer[max_buffer];
      CharString str;

      if (wantStdErr)
        cmdStr.append(" 2>&1");

#ifdef _WIN32
      stream = _popen(cmdStr.c_str(), "r");
#else
      stream = popen(cmdStr.c_str(), "r");
#endif
      if (stream)
      {
        while (!feof(stream))
        {
          if (fgets(buffer, max_buffer, stream) != 0)
          {
            str = buffer;
            str.trim(true);
            output.append(TextVar(str));
          }
        }
#ifdef _WIN32
        _pclose(stream);
#else
        pclose(stream);
#endif
      }
      else
        integerVar.setValue(-1);

      // directly assign the result to the target var
      *outputVar = output;

      integerVar = 0;
      return &integerVar;
    }

    // ---------------------------------------------------------------------
    default:
      integerVar.setValue(-1);
      return &integerVar;
  }
}

//--------------------------------------------------------------------------------
