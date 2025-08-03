char search_query_init(void);
char search_query_release(void);
char search_query_append(unsigned char c);
char search_query_delete_char(void);
char search_query_delete_range(unsigned int first, unsigned int last);
char search_query_rerun(void);
#define SEARCH_UNFILTERED 1
#define SEARCH_FILTERED 2
char search_collate(char min_score);
char search_sort_results_by_score(void);

