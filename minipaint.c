#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_c
{
	int		width;
	int		height;
	char	background;
	char	type;
	char	color;
	float	radius;
	float	x;
	float	y;
}t_c;

void	ft_free(FILE *file, char *draw)
{
	fclose(file);
	if (draw)
		free(draw);
}

int	check_zone(t_c *c)
{
	return ((c->width > 0 && c->width <= 300) && (c->height > 0 && c->height <= 300));
}

char	*getbackground(FILE *file, t_c *c)
{
	int		count, i = 0;
	char	*arr;
	if ((count = fscanf(file, "%d %d %c\n", &c->width, &c->height, &c->background)) != 3)
		return NULL;
	if (count == -1)
		return NULL;
	if (!(check_zone(c)))
		return NULL;
	arr = (char *)malloc(sizeof(char) * c->height * c->width);
	while (i < c->width * c->height)
		arr[i++] = c->background;
	return (arr);
}

int	is_rad(int x, int y, t_c *c)
{
	float	dist;
	dist = sqrtf(((x - c->x) * (x - c->x)) + ((y- c->y) * (y - c->y)));
	if (dist <= c->radius)
	{
		if (c->radius - dist < 1)
			return 2;
		return 1;
	}
	return 0;
}

void	get_draw(char **draw, t_c *c)
{
	int x, y = 0, rad;
	while (++y < c->height)
	{
		x = 0;
		while (++x < c->width)
		{
			rad = is_rad((float)x, (float)y, c);
			if ((c->type == 'c' && rad == 2) || (c->type == 'C' && rad))
				(*draw)[y * c->width + x] = c->color;
		}
	}
}

int	check_c(t_c *c)
{
	return ((c->radius > 0) && (c->type == 'c' || c->type == 'C'));
}

int	drawing(char **draw, FILE *file, t_c *c)
{
	int	count;
	while ((count = fscanf(file, "%c %f %f %f %c\n", &c->type, &c->x, &c->y, &c->radius, &c->color)) == 5)
	{
		if (!(check_c(c)))
			return 0;
		get_draw(draw, c);
	}
	if (count != (-1))
		return 0;
	return 1;
}

void	print_draw(char *draw, t_c *c)
{
	int i = 0;
	while (i < c->height)
	{
		write(1, draw + (i * c->width), c->width);
		write(1, "\n", 1);
		i++;
	}
}

int	main(int ac, char **av)
{
	char 	*draw;
	t_c		c;
	FILE	*file;
	if (ac != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (!(file = fopen(av[1], "r")))
	{
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	if (!(draw = getbackground(file, &c)))
	{
		ft_free(file, NULL);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	if (!(drawing(&draw, file, &c)))
	{
		ft_free(file, draw);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	print_draw(draw, &c);
	ft_free(file, draw);
	return (0);
}
