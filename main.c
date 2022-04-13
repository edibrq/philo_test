#include <pthread.h>
#include <stdio.h>

typedef pthread_mutex_t mutex;
typedef struct t_data_t
{
	mutex	*mtx;
	int		*data;
} data_t;

void *test(void *data)
{
	data_t *t = data;
	printf("data=%d\n", *t->data);
	pthread_mutex_lock(t->mtx);
	while (*t->data < 100000)
	{
		(*t->data)++;
		pthread_mutex_unlock(t->mtx);
		pthread_mutex_lock(t->mtx);
	}
	pthread_mutex_unlock(t->mtx);
	return (void *)1;
}

int main(void)
{	
	mutex		m;
	pthread_t	t;
	int			count;
	data_t		d;

	count = 0;
	pthread_mutex_init(&m, 0);
	d.data = &count;
	d.mtx = &m;
	pthread_create(&t, 0, test, &d);
	pthread_mutex_lock(d.mtx);
	while (count < 100000)
	{
		printf("%d\n", count);
		pthread_mutex_unlock(d.mtx);
		pthread_mutex_lock(d.mtx);
	}
	pthread_mutex_unlock(d.mtx);
	pthread_join(t, 0);
}
