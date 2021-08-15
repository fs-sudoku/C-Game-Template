#include "util.h"

void UTIL_PushInList(Node* node, void* value)
{
	Node* it = node;

	while (it->next != 0)
	{
		it = it->next;
	}
	it->next = malloc(sizeof(Node));
	UTIL_Verify(it->next);
	it->next->next = 0;
	it->next->value = value;
}