#!/bin/bash
exec 2> /dev/null
(
	rm *.o *~
	rm etape2/*.o *~
	rm etape3/*.o *~
	rm etape4/*.o *~
	rm my_shell/*.o *~
)
