typedef struct{
	int data;
	int parent;
	int eldest_c;
	int young_c;
	int young_sib;
	int children;
	bool visited;
} node;