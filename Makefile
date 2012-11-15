CC=g++

COMM_LIBS=atg_g.o cJSON.o util.o

PROGS=ex1 atgcat atgd t serial my

MY_INC!=mysql_config --include
MY_LIB!=mysql_config --libs

all:$(PROGS)

atgd:$(COMM_LIBS) atgd.c
	$(CC) -O $> -lpthread  -o $@

ex1:$(COMM_LIBS) ex1.c
	$(CC) $> -o $@

atgcat:atgcat.c
	$(CC) $> -o $@

t:t.c
	$(CC) $(COMM_LIBS) $> -o $@

.c.o:
	$(CC) -c $>

serial:serial.c
	$(CC) $> -lpthread -o serial
	
my:my.c
	$(CC) $> $(MY_INC) $(MY_LIB) -o my


clean:
	@-rm -rf $(COMM_LIBS)
	@-rm -rf $(PROGS)
	@-rm -rf *.o
