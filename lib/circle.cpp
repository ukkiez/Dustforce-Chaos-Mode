#include './std.cpp';
#include './drawing.cpp';

namespace drawing
{

	void circle(
		scene@ g, uint layer, uint sub_layer,
		const float x, const float y, const float radius, uint segments,
		const float thickness=2, uint colour=0xFFFFFFFF, const bool world=true)
	{
		if(segments < 3)
		{
			segments = 3;
		}

		float angle = 0;
		const float angle_step = PI * 2 / segments;

		for(uint i = 0; i < segments; i++)
		{
			draw_line(g,
				layer, sub_layer,
				x + cos(angle) * radius, y + sin(angle) * radius,
				x + cos(angle + angle_step) * radius, y + sin(angle + angle_step) * radius,
				thickness, colour, world);

			angle += angle_step;
		}
	}

	void fill_circle(
		scene@ g, uint layer, uint sub_layer,
		const float x, const float y, const float radius, const uint segments,
		const uint inner_colour, const uint outer_colour, const bool world=true, const float x_ratio=1)
	{
		const float step = PI2 / segments;
		float angle = step;

		float prev_x = x + radius * x_ratio;
		float prev_y = y;

		for(uint i = 0; i < segments; i += 2)
		{
			const float x2 = prev_x;
			const float y2 = prev_y;
			const float x3 = x + cos(angle) * radius * x_ratio;
			const float y3 = y + sin(angle) * radius;
			angle += step;
			const float x4 = i == segments - 1 ? x3 : x + cos(angle) * x_ratio * radius;
			const float y4 = i == segments - 1 ? y3 : y + sin(angle) * radius;
			angle += step;

			if(world)
			{
				g.draw_quad_world(layer, sub_layer, false,
					x, y, x2, y2, x3, y3, x4, y4,
					inner_colour, outer_colour, outer_colour, outer_colour);
			}
			else
			{
				g.draw_quad_hud(layer, sub_layer, false,
					x, y, x2, y2, x3, y3, x4, y4,
					inner_colour, outer_colour, outer_colour, outer_colour);
			}

			prev_x = x4;
			prev_y = y4;
		}
	}

	void fill_circle_outside(
		scene@ g, const uint layer, const uint sub_layer,
		const float x, const float y, const float radius, const uint segments,
		const uint inside_colour, const uint outside_colour, const float outside_size=7000, const float x_ratio=1)
	{
		float prev_x = x + radius * x_ratio;
		float prev_y = y;
		const float step = PI2 / segments;
		float angle = step;
		const float rf = 1 / radius * outside_size;

		for(uint i = 0; i < segments; i++)
		{
			const float cx = x + cos(i == segments - 1 ? 0 : angle) * radius * x_ratio;
			const float cy = y + sin(i == segments - 1 ? 0 : angle) * radius;

			g.draw_quad_world(layer, sub_layer, false,
				prev_x, prev_y,
				cx, cy,
				cx + (cx - x) * rf, cy + (cy - y) * rf,
				prev_x + (prev_x - x) * rf, prev_y + (prev_y - y) * rf,
				inside_colour, inside_colour, outside_colour, outside_colour);

			prev_x = cx;
			prev_y = cy;
			angle += step;
		}
	}

	void fill_ring(
		scene@ g, const uint layer, const uint sub_layer,
		const float x, const float y, const float inner_radius, const float outer_radius, const uint segments,
		const uint inside_colour, const uint outside_colour, const float x_ratio=1)
	{
		float prev_cx = 1 * x_ratio;
		float prev_cy = 0;
		const float step = PI2 / segments;
		float angle = step;

		for(uint i = 0; i < segments; i++)
		{
			const float cx = cos(i == segments - 1 ? 0 : angle) * x_ratio;
			const float cy = sin(i == segments - 1 ? 0 : angle);

			g.draw_quad_world(layer, sub_layer, false,
				x + prev_cx * inner_radius, y + prev_cy * inner_radius,
				x + cx * inner_radius, y + cy * inner_radius,
				x + cx * outer_radius, y + cy * outer_radius,
				x + prev_cx * outer_radius, y + prev_cy * outer_radius,
				inside_colour, inside_colour, outside_colour, outside_colour);

			prev_cx = cx;
			prev_cy = cy;
			angle += step;
		}
	}

	void arc(
		scene@ g, uint layer, uint sub_layer,
		const float x, const float y, const float size_x, const float size_y,
		const float start, const float end, uint segments,
		const float thickness=2, const uint colour=0xFFFFFFFF)
	{
		if(segments < 3)
		{
			segments = 3;
		}

		const float angle_step = PI2 / segments;
		const float angle_start = start * DEG2RAD;
		const float angle_end = end * DEG2RAD;
		float angle_delta = angle_end - angle_start;

		if(angle_delta > PI2)
		{
			angle_delta = PI2;
		}
		else if(angle_delta < -PI2)
		{
			angle_delta = -PI2;
		}

		const int steps = clamp(ceil_int(abs(angle_delta / angle_step)), 2, segments);

		float prev_x = x + cos(angle_start) * size_x;
		float prev_y = y + sin(angle_start) * size_y;

		for(int i = 1; i < steps; i++)
		{
			float t = float(i) / (steps - 1);
			float angle = angle_start + angle_delta * t;

			float px = x + cos(angle) * size_x;
			float py = y + sin(angle) * size_y;

			draw_line(g,
				layer, sub_layer,
				prev_x, prev_y,
				px, py,
				thickness, colour);

			prev_x = px;
			prev_y = py;
		}
	}


}
