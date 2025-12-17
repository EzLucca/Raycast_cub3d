/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:19:38 by edlucca           #+#    #+#             */
/*   Updated: 2025/09/04 11:24:12 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

//------------------------------------------------------------------------------
// Library Headers
//------------------------------------------------------------------------------

# include <limits.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

//------------------------------------------------------------------------------
// Macro Definitions
//------------------------------------------------------------------------------

// Delimiters for tokenization.
# define DELIMITERS "|<> \t\n"

// Return / exit values (general).
# define SUCCESS 0
# define FAILURE 1

//------------------------------------------------------------------------------
// Type Definitions
//------------------------------------------------------------------------------

// Data struct typedef from "minishell.h".
struct					s_data;
typedef struct s_data	t_data;

// Token type enumeration.
typedef enum e_token_type
{
	TOK_PIPE,
	TOK_IN,
	TOK_OUT,
	TOK_HERE,
	TOK_HERE_QTD,
	TOK_HERE_UNQTD,
	TOK_APP,
	TOK_WORD,
	TOK_CMD,
	TOK_ARG
}	t_token_type;

// Environmental variable structure.
typedef struct s_env
{
	char			*key;
	char			*value;
	bool			assigned;
	struct s_env	*next;
}	t_env;

// Parsing token structure.
typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
}	t_token;

// Execution token structure.
typedef struct s_exec
{
	char			**cmd_arg;
	char			**in;
	char			**out;
	char			**app;
	bool			in_first;
	bool			out_passed;
	int				infile;	
	int				outfile;	
	int				fd[2];
	int				saved_stdin;
	int				saved_stdout;
	struct s_exec	*next;
}	t_exec;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

/**
 * @brief Exit function for overflow in ft_atoi().
 * @param data Data struct of all core variables.
 * @return None.
 */
void	exit_atoi_overflow(t_data *data);

/**
 * @brief Converts a string into an integer.
 * @param nptr String to convert.
 * @param data Data struct of all core variables.
 * @return Converted string as int.
 */
int		ft_atoi(const char *nptr, t_data *data);

/**
 * @brief Erases data in a memory by writing \0 to that area.
 * @param s Pointer to the starting location.
 * @param n Bytes of the memory to be erased.
 * @return None.
 */
void	ft_bzero(void *s, size_t n);

/**
 * @brief Allocates memory for an array and sets that memory to zero.
 * @param nmemb Number of elements in the array.
 * @param size Size in bytes of each element.
 * @return Void pointer to the memory area, NULL on any error.
 */
void	*ft_calloc(size_t nmemb, size_t size);

/**
 * @brief Fully frees an array of arrays.
 * @param array Array to free.
 * @return None.
 */
void	ft_free_array(char **array);

/** Checks for an alphanumeric character.
 * @param c Character to check, passed as an int.
 * @return Bool true or false.
 */
bool	ft_isalnum(int c);

/** Checks for an alphabetic character.
 * @param c Character to check, passed as an int.
 * @return Bool true or false.
 */
bool	ft_isalpha(int c);

/**
 * @brief Checks if a character is a delimiter and sets len accordingly.
 * @param s String to check.
 * @param i Index of a character.
 * @param len Length of a token.
 * @return Bool true or false.
 */
bool	ft_isdelimiter(char *s, size_t i, size_t *len);

/**
 * @brief Checks if a character is a digit.
 * @param c Character to check, passed as an int.
 * @return Bool true or false.
 */
bool	ft_isdigit(int c);

/** Checks if a character is a dollar sign and outside of single quotes.
 * @param c Character to check, passed as an int.
 * @param quote Current open quote.
 * @return Bool true or false.
 */
bool	ft_isexpandable(int c, char *quote);

/**
 * @brief Checks if a character is an exception from an expansion.
 * @param content Content of the current token.
 * @param quote Current tracked quote state.
 * @param i Index of a character.
 * @return Bool true or false.
 */
bool	ft_isexpexception(char *content, char quote, size_t i);

/**
 * @brief Checks if a character is an input field separator.
 * @param c Character to check, passed as an int.
 * @return Bool true or false.
 */
bool	ft_isifs(int c);

/**
 * @brief Checks if a character is a quote (single or double).
 * @param c Character to check, passed as an int.
 * @return Bool true or false.
 */
bool	ft_isquote(int c);

/**
 * @brief Converts an integer to a string.
 * @param n Integer to convert.
 * @return String, NULL if allocation fails.
 */
char	*ft_itoa(int n);

/**
 * @brief Adds a node at the end of a linked list.
 * @param lst Pointer to the head pointer of a linked list.
 * @param node Pointer to a node to be added.
 * @return None.
 */
void	ft_lst_env_add_back(t_env **lst, t_env *node);

/**
 * @brief Frees a given node, its content, and all its successors.
 * @param lst Pointer to the pointer of a node.
 * @return None.
 */
void	ft_lst_env_clear(t_env **lst);

/** Returns a last node of a list.
 * @param lst Pointer to a first node of a linked list.
 * @return Pointer to a last node.
 */
t_env	*ft_lst_env_last(t_env *lst);

/**
 * @brief Creates a new node in a linked list.
 * @param key Key string.
 * @param value Value string.
 * @return Pointer to a node, NULL if allocation fails.
 */
t_env	*ft_lst_env_new(void *key, void *value);

/**
 * @brief Removes a node from a linked list while preserving the links.
 * @param lst  Pointer to the head pointer of a linked list.
 * @param node Pointer to a node to be removed.
 * @return None.
 */
void	ft_lst_env_remove(t_env **lst, t_env *node);

/** Returns a size of a linked list.
 * @param lst Pointer to a first node of a linked list.
 * @return Size of a list.
 */
size_t	ft_lst_env_size(t_env *lst);

/** Sorts an array of env by ASCII values.
 * @param arr Array of pointers to env structs.
 * @param size Size of the array.
 * @return None.
 */
void	ft_lst_env_sort(t_env **arr, size_t size);

/**
 * @brief Adds a node at the end of a linked list.
 * @param lst Pointer to the head pointer of a linked list.
 * @param node Pointer to a node to be added.
 * @return None.
 */
void	ft_lst_exec_add_back(t_exec **lst, t_exec *node);

/**
 * @brief Frees a given node, its content, and all its successors.
 * @param lst Pointer to the pointer of a node.
 * @return None.
 */
void	ft_lst_exec_clear(t_exec **lst);

/** Returns a last node of a list.
 * @param lst Pointer to a first node of a linked list.
 * @return Pointer to a last node.
 */
t_exec	*ft_lst_exec_last(t_exec *lst);

/**
 * @brief Creates a new node in a linked list.
 * @param cmd_arg Array of commands and arguments.
 * @param in Array of redirections in.
 * @param out Array of redirections out.
 * @return Pointer to a node, NULL if allocation fails.
 */
t_exec	*ft_lst_exec_new(char **cmd_arg, char **in, char **out, char **app);

/**
 * @brief Adds a node at the end of a linked list.
 * @param lst Pointer to the head pointer of a linked list.
 * @param node Pointer to a node to be added.
 * @return None.
 */
void	ft_lst_tok_add_back(t_token **lst, t_token *node);

/**
 * @brief Frees a given node, its content, and all its successors.
 * @param lst Pointer to the pointer of a node.
 * @return None.
 */
void	ft_lst_tok_clear(t_token **lst);

/** Returns a last node of a list.
 * @param lst Pointer to a first node of a linked list.
 * @return Pointer to a last node.
 */
t_token	*ft_lst_tok_last(t_token *lst);

/**
 * @brief Creates a new node in a linked list.
 * @param content Token string.
 * @return Pointer to a node, NULL if allocation fails.
 */
t_token	*ft_lst_tok_new(void *content);

/**
 * @brief Removes a node from a linked list while preserving the links.
 * @param lst  Pointer to the head pointer of a linked list.
 * @param node Pointer to a node to be removed.
 * @return None.
 */
void	ft_lst_tok_remove(t_token **lst, t_token *node);

/**
 * @brief Copies bytes from one memory area to another.
 * @param dest Pointer to the destination memory area.
 * @param src Pointer to the source memory area.
 * @param n Number of bytes to copy.
 * @return Pointer to the destination memory area.
 */
void	*ft_memcpy(void *dst, const void *src, size_t n);

/**
 * @brief Fills a memory area with a constant byte c.
 * @param s Pointer to the memory area.
 * @param c Character to write into each byte, passed as an int.
 * @param n Number of bytes to fill.
 * @return Pointer to the memory area.
 */
void	*ft_memset(void *s, int c, size_t n);

/**
 * @brief Outputs a character to a specified file descriptor.
 * @param c Character to output.
 * @param fd File descriptor.
 * @return None.
 */
void	ft_putchar_fd(char c, int fd);

/**
 * @brief Outputs a string to a specified file descriptor, followed by a newline.
 * @param s String to output.
 * @param fd File descriptor.
 * @return None.
 */
void	ft_putendl_fd(char *s, int fd);

/**
 * @brief Outputs strings to a specified file descriptor, followed by a newline.
 * @param s1 String to output.
 * @param s2 String to output.
 * @param fd File descriptor.
 * @return None.
 */
void	ft_putendl_fd2(char *s1, char *s2, int fd);

/**
 * @brief Outputs an integer to a specified file descriptor.
 * @param n Integer to output.
 * @param fd File descriptor.
 * @return None.
 */
void	ft_putnbr_fd(int n, int fd);

/**
 * @brief Outputs a string to a specified file descriptor.
 * @param s String to output.
 * @param fd File descriptor.
 * @return None.
 */
void	ft_putstr_fd(char *s, int fd);

/**
 * @brief Splits a string according to a specified delimiter.
 * @param s String to split.
 * @param c Delimiter.
 * @return Array of new strings, NULL if the allocation fails.
 */
char	**ft_split(char const *s, char c);

/**
 * @brief Scans a string for the first instance of a character.
 * @param s String to search.
 * @param c Character to search for, passed as an int.
 * @return Pointer to the matching location, NULL if no match.
 */
char	*ft_strchr(const char *s, int c);

/**
 * @brief Compares two strings.
 * @param s1 First string.
 * @param s2 Second string.
 * @return 0 if equal, negative if s1 < s2, positive if s1 > s2.
 */
int		ft_strcmp(const char *s1, const char *s2);

/**
 * @brief Duplicates a string using dynamic memory allocation.
 * @param s String to duplicate.
 * @return Pointer to the duplicated string, NULL if the allocation fails.
 */
char	*ft_strdup(const char *s);

/**
 * @brief Concatenates two strings using dynamic memory allocation.
 * @param s1 First string.
 * @param s2 Second string.
 * @return Pointer to the new string, 'NULL' if the allocation fails.
 */
char	*ft_strjoin(char const *s1, char const *s2);

/**
 * @brief Concatenates two strings and NULL-terminates the result.
 * @param dst Destination string.
 * @param src String to append to the destination string.
 * @param size Number of bytes to append (includes byte for NUL).
 * @return Length of the string it tried to create.
 */
size_t	ft_strlcat(char *dst, const char *src, size_t size);

/**
 * @brief Copies a string and NULL-terminates the result.
 * @param dst Destination array.
 * @param scr Source string to copy to the detination array.
 * @param size Number of byte to copy.
 * @return Length of the string it tried to create.
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t size);

/**
 * @brief Calculates the length of a string.
 * @param s String to calculate the length of.
 * @return Number of bytes in the passed string.
 */
size_t	ft_strlen(const char *s);

/**
 * @brief Compares two strings limited by the number of bytes.
 * @param s1 First string.
 * @param s2 Second string.
 * @param n Number of bytes to compare.
 * @return 0 if equal, negative if s1 < s2, positive if s1 > s2.
 */
int		ft_strncmp(const char *s1, const char *s2, size_t n);

/**
 * @brief Locates the first occurrence of a string in another string.
 * @param big String to search in.
 * @param little String to search for.
 * @param len Number of bytes to search.
 * @return Pointer to the first character of the first occurrence.
 */
char	*ft_strnstr(const char *big, const char *little, size_t len);

/**
 * @brief Scans a string for the last instance of a character.
 * @param s String to search.
 * @param c Character to search for, passed as an int.
 * @return Pointer to the matching location, NULL if no match.
 */
char	*ft_strrchr(const char *s, int c);

/**
 * @brief Creates a substring using dynamic memory allocation.
 * @param s String to make the substring from.
 * @param start Starting index.
 * @param len Length of the substring.
 * @return Pointer to the new substring, NULL if the allocation fails.
 */
char	*ft_substr(char const *s, unsigned int start, size_t len);

/**
 * @brief Converts uppercase letter to lowercase.
 * @param c Character to convert, passed as an int.
 * @return Converted character.
 */
int		ft_tolower(int c);

/**
 * @brief Converts lowercase letter to uppercase.
 * @param c Character to convert, passed as an int.
 * @return Converted character.
 */
int		ft_toupper(int c);

/**
 * @brief Specifies and returns which input field separator was found.
 * @param c Character to check, passed as an int.
 * @return Specified input field separator.
 */
char	ft_whichifs(int c);

/* GNL*********************************************************************** */

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10 
# endif

char	*ft_strtrim(char const *s1, char const *set);

char	*ft_substr_gnl(char const *src, unsigned int start, size_t len);
size_t	ft_strlen_gnl(const char *s);
char	*ft_strjoin_gnl(char const *s1, char const *s2);
char	*ft_strchr_gnl(char *s, char c);
char	*ft_strdup_copy(const char *src);
char	*get_next_line(int fd);

#endif
