final class Box {
	float left;
	float top;
	float right;
	float bottom;

	Box( float left = 0, float top = 0, float right = 0, float bottom = 0 ) {
		this.left = left;
		this.top = top;
		this.bottom = bottom;
		this.right = right;
	}

	void set( float left, float top, float right, float bottom ) {
		this.left = left;
		this.top = top;
		this.right = right;
		this.bottom = bottom;
	}

	bool is_inside( float box_pos_x, float box_pos_y, float x, float y ) {
		return ( x >= box_pos_x + left ) &&
    ( x <= box_pos_x + right ) &&
    ( y >= box_pos_y + top ) &&
    ( y <= box_pos_y + bottom );
	}
}
