.PHONY : all clean rebuild

SRC := *.c
OUT := bt.o
LOG := btsnoop.log
# IMG := data.img

RM := rm -rf

all : $(OUT)
	@echo "Sucess!"

run :
	sudo ./bt.o

btsnoop : 
	sudo chmod 777 btsnoop.log
	
$(OUT) : $(SRC)
	gcc $^ -o $@ -lpthread
	
clean : 
	$(RM) $(OUT) $(LOG)
	
rebuild : 
	@$(MAKE) clean
	@$(MAKE) all
