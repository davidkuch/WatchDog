#include "uid.h"
#include <stdlib.h>



const uid_ty BAD_UID = {0};

uid_ty UIDGenerate(void)
{
	static size_t counter = 0;
	uid_ty uid;
	

	
	uid.counter = counter; /* TODO : ATOMIC */
	uid.time = time(NULL);
	uid.pid = getpid();
	
	++counter;
	
	return uid;
}


/* O(1)  */
int UIDIsSame(uid_ty uid1, uid_ty uid2)
{
	return (uid1.counter == uid2.counter && 
			uid1.time == uid2.time && 
			uid1.pid == uid2.pid);
}

int UIDIsBad(uid_ty uid)
{
	return  UIDIsSame(uid, BAD_UID);
}

