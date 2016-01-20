include $(API_ROOT)/CtrlExt.mk

OFILES = SysAdvExternHdl.o

CtrlSysAdv: $(OFILES)
	@rm -f addVerInfo.o
	@$(MAKE) addVerInfo.o
	$(SHLIB) -o CtrlSysAdv.so addVerInfo.o $(OFILES) $(LIBS)

clean:
	@rm -f *.o CtrlSysAdv.so

addVerInfo.cxx: $(API_ROOT)/addVerInfo.cxx
	@cp -f $(API_ROOT)/addVerInfo.cxx addVerInfo.cxx

addVerInfo.o: $(OFILES) addVerInfo.cxx
