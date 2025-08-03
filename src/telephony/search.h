char search_query_init(void);
char search_query_release(void);
char search_query_append(unsigned char c);
char search_query_delete_char(void);
char search_query_delete_range(unsigned int first, unsigned int last);
char search_collate(void);
char search_sort_results_by_score(void);


