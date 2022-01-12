#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_r
{
	int		width;
	int		height;
	char	background;
	char	type;
	char	color;
	float	width_r;
	float	height_r;
	float	x;
	float	y;
}t_r;

void	ft_free(FILE *file, char *draw)
{
	fclose(file);
	if (draw)
		free(draw);
}

int	check_zone(t_r *r)
{
	return ((r->width > 0 && r->width <= 300) && (r->height > 0 && r->height <= 300));
}

char	*getbackground(FILE *file, t_r *r)
{
	int		count;
	char	*arr;
	int i = 0;
	if ((count = fscanf(file, "%d %d %c\n", &r->width, &r->height, &r->background)) != 3)
		return NULL;
	if (count == -1)
		return NULL;
	if (!(check_zone(r)))
		return NULL;
	arr = (char *)malloc(sizeof(char) * r->height * r->width);
	while (i < r->width * r->height)
		arr[i++] = r->background;
	return (arr);
}

int	check_r(t_r *r)
{
	return ((r->width_r > 0 && r->height_r > 0) && (r->type == 'r' || r->type == 'R'));
}

int	is_rec(int x, int y, t_r *r)
{
	if ((x < r->x) || (r->x + r->width_r < x) || (y < r->y) || (r->y + r->height_r) < y)
		return 0;
	if ((x - r->x < 1) || (r->x + r->width_r - x < 1) || (y - r->y < 1) || (r->y + r->height_r - y < 1))
		return 2;
	return 1;
}

void	get_draw(char *draw, t_r *r)
{
	int x, y = 0, rec;
	while (++y < r->height)
	{
		x = 0;
		while (++x < r->width)
		{
			rec = is_rec(x, y, r);
			if ((r->type == 'r' && rec == 2) || (r->type == 'R' && rec))
				draw[y * r->width + x] = r->color;
		}
	}
}

int	drawing(char *draw, FILE *file, t_r *r)
{
	int count;
	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &r->type, &r->x, &r->y, &r->width_r, &r->height_r, &r->color)) == 6)
	{
		if (!(check_r(r)))
			return 0;
		get_draw(draw, r);
	}
	if (count != (-1))
		return 0;
	return 1;
}

void	print_draw(char *draw, t_r *r)
{
	int i = 0;
	while (i < r->height)
	{
		write(1, draw + (i * r->width), r->width);
		write(1, "\n", 1);
		i++;
	}
}

int	main(int ac, char **av)
{
	char 	*draw;
	t_r		r;
	FILE	*file;
	if (ac != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (!(file = fopen(av[1], "r")))
	{
		write(1, "Error: Operation file corupted\n", 34);
		return (1);
	}
	if (!(draw = getbackground(file, &r)))
	{
		ft_free(file, NULL);
		write(1, "Error: Operation file corupted\n", 34);
		return (1);
	}
	if (!(drawing(draw, file, &r)))
	{
		ft_free(file, draw);
		write(1, "Error: Operation file corupted\n", 34);
		return (1);
	}
	print_draw(draw, &r);
	ft_free(file, draw);
	return (0);
}
