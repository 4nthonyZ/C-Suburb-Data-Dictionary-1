CC = gcc
CFLAGS = -Wall -g

TARGETS = dict1 dict2

SRCS_DICT1 = dict1.c Linked_list.c
SRCS_DICT2 = dict2.c Linked_list.c

dict1: $(SRCS_DICT1)
	$(CC) $(CFLAGS) -o dict1 $(SRCS_DICT1)

dict2: $(SRCS_DICT2)
	$(CC) $(CFLAGS) -o dict2 $(SRCS_DICT2)

test1:
	./dict1 1 tests/dataset_1.csv output.out < tests/test1.in

test15:
	./dict1 1 tests/dataset_15.csv output.out < tests/test15.in

test100:
	./dict1 1 tests/dataset_100.csv output.out < tests/test100.in

test1000:
	./dict1 1 tests/dataset_1000.csv output.out < tests/test1000.in

testfull:
	./dict1 1 tests/dataset_full.csv output.out < tests/testfull.in

del1:
	./dict2 2 tests/dataset_del.csv output.out < tests/del1.in

del2:
	./dict2 2 tests/dataset_del.csv output.out < tests/del2.in

del3:
	./dict2 2 tests/dataset_del.csv output.out < tests/del3.in

clean:
	rm -f $(TARGETS) output.out
