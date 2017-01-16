#define MAX 260

struct htype {
	int index;   /* логический индекс */
	int val;     /* собственно значение элемента данных */
	struct htype *next; /* указатель на следующий элемент с таким же
						хэш-адресом */
} primary[MAX];
