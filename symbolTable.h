
struct identifier
{
	char* name;
	int offset;
	int type; //int=0 or real=1
	int size;
	struct identifier* next;
	char *tname;
};
typedef struct identifier identifier;


struct idtable {
	int size;
	identifier **table;
};
typedef struct idtable idtable;


struct func
{
	char* name;
	int childNo;;
	int offset;
	int noIn;
	int noOut;
	int size;
	int *inType, *outType;
	idtable* localtable;
	struct func* next;
};
typedef struct func func;


struct recordtype
{
	char* name;
	int type;
	int size;
	int noField;
	int* fieldtype;
	char** fieldid;
	struct recordtype*next;
};
typedef struct recordtype recordtype;

/**************HASHTABLES****************/


struct ftable
{
	int size;
	func **table;
};
typedef struct ftable ftable;



struct typetable
{
	int size;
	recordtype **table;
};
typedef struct typetable typetable;
