idtable *idcreateHashtable( int size );
int idhash( idtable *hashtable,char *str );
typetable *tcreateHashtable( int size );
ftable *fcreateHashtable( int size );
idtable *idcreateHashtable( int size ) ;
recordtype* thash_get( typetable *hashtable, char *name );
func* fhash_get( ftable *hashtable, char *name );
identifier *idNewpair( char* name, int offset, int type, int size, char *tname);