#define MAX 260

struct htype {
	int index;   /* ���������� ������ */
	int val;     /* ���������� �������� �������� ������ */
	struct htype *next; /* ��������� �� ��������� ������� � ����� ��
						���-������� */
} primary[MAX];
