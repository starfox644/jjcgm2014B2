#!/bin/bash
exec 2> /dev/null
(
	rm *.o *~
	rm */*.o *~
)
