/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharras <jharras@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 23:43:10 by jharras           #+#    #+#             */
/*   Updated: 2022/06/02 23:43:27 by jharras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*is_dead(void *arg)
{
	t_p	*p;
	int	stop;

	p = (t_p *)arg;
	stop = 0;
	while (!stop)
	{
		pthread_mutex_lock(&p->info->m_last_eat);
		if (get_time() - p->t_last_eat >= p->t_die)
		{
			pthread_mutex_unlock(&p->info->m_last_eat);
			pthread_mutex_lock(&p->info->m_msg);
			print_status(p, "died");
			pthread_mutex_unlock(&p->info->m_msg);
		}
		else
			pthread_mutex_unlock(&p->info->m_last_eat);
		pthread_mutex_lock(&p->info->m_stop);
		stop = p->info->stop + p->stop;
		pthread_mutex_unlock(&p->info->m_stop);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_p	*p;
	int	stop;

	p = (t_p *)arg;
	p->t_last_eat = p->info->t_start;
	if (pthread_create(&p->faucheuse, NULL, &is_dead, p))
		perror("pthread_create failed");
	stop = 0;
	while (!stop)
	{
		activity(p);
		pthread_mutex_lock(&p->info->m_stop);
		stop = p->info->stop + p->stop;
		pthread_mutex_unlock(&p->info->m_stop);
	}
	if (pthread_join(p->faucheuse, NULL))
		perror("pthread_join failed");
	return (NULL);
}

void	destroy_mutex(t_info *info, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < info->n_philo)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&info->m_last_eat);
	pthread_mutex_destroy(&info->m_msg);
	pthread_mutex_destroy(&info->m_stop);
}

int	launching_threading(t_p *philos, t_info *info, pthread_t *th)
{
	int	i;

	i = 0;
	info->t_start = get_time();
	while (i < info->n_philo)
	{
		if (pthread_create(&th[i], NULL, &routine, &philos[i]))
			perror("pthread_create failed");
		i++;
	}
	i = 0;
	while (i < info->n_philo)
	{
		if (pthread_join(th[i], NULL))
			perror("pthread_join failed");
		i++;
	}
	return (0);
}
