class PropReference {
  uint set;
  uint group;
  uint index;
  PropReference( uint set, uint group, uint index ) {
    this.set = set;
    this.group = group;
    this.index = index;
  }

  PropReference() {}
}

const array<PropReference> prop_references = {
	PropReference( 1, 22, 1 ),
	PropReference( 1, 22, 2 ),
	PropReference( 1, 22, 3 ),
	PropReference( 1, 22, 4 ),
	PropReference( 1, 22, 5 ),
	PropReference( 1, 22, 6 ),
	PropReference( 2, 22, 1 ),
	PropReference( 2, 22, 2 ),
	PropReference( 2, 22, 3 ),
	PropReference( 2, 22, 4 ),
	PropReference( 2, 22, 5 ),
	PropReference( 2, 22, 6 ),
	PropReference( 2, 22, 7 ),
	PropReference( 3, 22, 1 ),
	PropReference( 3, 22, 2 ),
	PropReference( 3, 22, 3 ),
	PropReference( 3, 22, 4 ),
	PropReference( 3, 22, 5 ),
	PropReference( 3, 22, 6 ),
	PropReference( 3, 22, 7 ),
	PropReference( 3, 22, 8 ),
	PropReference( 4, 22, 1 ),
	PropReference( 4, 22, 2 ),
	PropReference( 4, 22, 3 ),
	PropReference( 2, 18, 1 ),
	PropReference( 2, 18, 2 ),
	PropReference( 2, 18, 3 ),
	PropReference( 2, 18, 4 ),
	PropReference( 2, 18, 5 ),
	PropReference( 2, 18, 6 ),
	PropReference( 2, 18, 7 ),
	PropReference( 2, 18, 8 ),
	PropReference( 1, 0, 10 ),
	PropReference( 1, 0, 1 ),
	PropReference( 1, 0, 2 ),
	PropReference( 1, 0, 3 ),
	PropReference( 1, 0, 4 ),
	PropReference( 1, 0, 5 ),
	PropReference( 1, 0, 6 ),
	PropReference( 1, 0, 7 ),
	PropReference( 1, 0, 8 ),
	PropReference( 1, 0, 9 ),
	PropReference( 2, 21, 10 ),
	PropReference( 2, 21, 11 ),
	PropReference( 2, 21, 12 ),
	PropReference( 2, 21, 13 ),
	PropReference( 2, 21, 14 ),
	PropReference( 2, 21, 15 ),
	PropReference( 2, 21, 16 ),
	PropReference( 2, 21, 1 ),
	PropReference( 2, 21, 2 ),
	PropReference( 2, 21, 3 ),
	PropReference( 2, 21, 4 ),
	PropReference( 2, 21, 5 ),
	PropReference( 2, 21, 6 ),
	PropReference( 2, 21, 7 ),
	PropReference( 2, 21, 8 ),
	PropReference( 2, 21, 9 ),
	PropReference( 1, 1, 10 ),
	PropReference( 1, 1, 1 ),
	PropReference( 1, 1, 2 ),
	PropReference( 1, 1, 3 ),
	PropReference( 1, 1, 4 ),
	PropReference( 1, 1, 5 ),
	PropReference( 1, 1, 6 ),
	PropReference( 1, 1, 7 ),
	PropReference( 1, 1, 8 ),
	PropReference( 1, 1, 9 ),
	PropReference( 3, 26, 1 ),
	PropReference( 3, 26, 2 ),
	PropReference( 3, 26, 3 ),
	PropReference( 1, 2, 1 ),
	PropReference( 1, 2, 2 ),
	PropReference( 1, 2, 3 ),
	PropReference( 1, 2, 4 ),
	PropReference( 1, 2, 5 ),
	PropReference( 1, 3, 1 ),
	PropReference( 1, 3, 2 ),
	PropReference( 1, 3, 3 ),
	PropReference( 1, 3, 4 ),
	PropReference( 1, 3, 5 ),
	PropReference( 1, 3, 6 ),
	PropReference( 1, 3, 7 ),
	PropReference( 1, 3, 8 ),
	PropReference( 1, 3, 9 ),
	PropReference( 2, 3, 1 ),
	PropReference( 1, 4, 1 ),
	PropReference( 1, 4, 2 ),
	PropReference( 1, 4, 3 ),
	PropReference( 3, 4, 10 ),
	PropReference( 3, 4, 11 ),
	PropReference( 3, 4, 12 ),
	PropReference( 3, 4, 13 ),
	PropReference( 3, 4, 14 ),
	PropReference( 3, 4, 1 ),
	PropReference( 3, 4, 2 ),
	PropReference( 3, 4, 3 ),
	PropReference( 3, 4, 4 ),
	PropReference( 3, 4, 5 ),
	PropReference( 3, 4, 6 ),
	PropReference( 3, 4, 7 ),
	PropReference( 3, 4, 8 ),
	PropReference( 3, 4, 9 ),
	PropReference( 1, 13, 1 ),
	PropReference( 1, 13, 2 ),
	PropReference( 1, 13, 3 ),
	PropReference( 1, 13, 4 ),
	PropReference( 1, 13, 5 ),
	PropReference( 1, 13, 6 ),
	PropReference( 1, 5, 10 ),
	PropReference( 1, 5, 11 ),
	PropReference( 1, 5, 12 ),
	PropReference( 1, 5, 13 ),
	PropReference( 1, 5, 14 ),
	PropReference( 1, 5, 15 ),
	PropReference( 1, 5, 16 ),
	PropReference( 1, 5, 17 ),
	PropReference( 1, 5, 18 ),
	PropReference( 1, 5, 19 ),
	PropReference( 1, 5, 1 ),
	PropReference( 1, 5, 20 ),
	PropReference( 1, 5, 2 ),
	PropReference( 1, 5, 3 ),
	PropReference( 1, 5, 4 ),
	PropReference( 1, 5, 5 ),
	PropReference( 1, 5, 6 ),
	PropReference( 1, 5, 7 ),
	PropReference( 1, 5, 8 ),
	PropReference( 1, 5, 9 ),
	PropReference( 2, 5, 10 ),
	PropReference( 2, 5, 11 ),
	PropReference( 2, 5, 12 ),
	PropReference( 2, 5, 13 ),
	PropReference( 2, 5, 14 ),
	PropReference( 2, 5, 15 ),
	PropReference( 2, 5, 16 ),
	PropReference( 2, 5, 17 ),
	PropReference( 2, 5, 18 ),
	PropReference( 2, 5, 19 ),
	PropReference( 2, 5, 1 ),
	PropReference( 2, 5, 20 ),
	PropReference( 2, 5, 21 ),
	PropReference( 2, 5, 22 ),
	PropReference( 2, 5, 23 ),
	PropReference( 2, 5, 24 ),
	PropReference( 2, 5, 25 ),
	PropReference( 2, 5, 26 ),
	PropReference( 2, 5, 2 ),
	PropReference( 2, 5, 3 ),
	PropReference( 2, 5, 4 ),
	PropReference( 2, 5, 5 ),
	PropReference( 2, 5, 6 ),
	PropReference( 2, 5, 7 ),
	PropReference( 2, 5, 8 ),
	PropReference( 2, 5, 9 ),
	PropReference( 1, 6, 1 ),
	PropReference( 1, 6, 2 ),
	PropReference( 1, 6, 3 ),
	PropReference( 1, 6, 4 ),
	PropReference( 1, 6, 5 ),
	PropReference( 1, 6, 6 ),
	PropReference( 1, 6, 7 ),
	PropReference( 1, 6, 8 ),
	PropReference( 1, 6, 9 ),
	PropReference( 1, 7, 1 ),
	PropReference( 1, 7, 2 ),
	PropReference( 1, 7, 3 ),
	PropReference( 1, 7, 4 ),
	PropReference( 1, 7, 5 ),
	PropReference( 1, 7, 6 ),
	PropReference( 1, 7, 7 ),
	PropReference( 2, 19, 1 ),
	PropReference( 2, 19, 2 ),
	PropReference( 2, 19, 3 ),
	PropReference( 2, 19, 4 ),
	PropReference( 2, 19, 5 ),
	PropReference( 2, 19, 6 ),
	PropReference( 2, 19, 7 ),
	PropReference( 2, 19, 8 ),
	PropReference( 1, 8, 10 ),
	PropReference( 1, 8, 11 ),
	PropReference( 1, 8, 1 ),
	PropReference( 1, 8, 2 ),
	PropReference( 1, 8, 3 ),
	PropReference( 1, 8, 4 ),
	PropReference( 1, 8, 5 ),
	PropReference( 1, 8, 6 ),
	PropReference( 1, 8, 7 ),
	PropReference( 1, 8, 8 ),
	PropReference( 1, 8, 9 ),
	PropReference( 2, 8, 1 ),
	PropReference( 2, 8, 2 ),
	PropReference( 3, 8, 1 ),
	PropReference( 3, 8, 2 ),
	PropReference( 4, 28, 10 ),
	PropReference( 4, 28, 11 ),
	PropReference( 4, 28, 12 ),
	PropReference( 4, 28, 13 ),
	PropReference( 4, 28, 14 ),
	PropReference( 4, 28, 15 ),
	PropReference( 4, 28, 16 ),
	PropReference( 4, 28, 17 ),
	PropReference( 4, 28, 18 ),
	PropReference( 4, 28, 19 ),
	PropReference( 4, 28, 1 ),
	PropReference( 4, 28, 2 ),
	PropReference( 4, 28, 3 ),
	PropReference( 4, 28, 4 ),
	PropReference( 4, 28, 5 ),
	PropReference( 4, 28, 6 ),
	PropReference( 4, 28, 7 ),
	PropReference( 4, 28, 8 ),
	PropReference( 4, 28, 9 ),
	PropReference( 5, 24, 1 ),
	PropReference( 6, 24, 1 ),
	PropReference( 6, 24, 2 ),
	PropReference( 6, 24, 3 ),
	PropReference( 6, 24, 4 ),
	PropReference( 6, 24, 5 ),
	PropReference( 3, 27, 10 ),
	PropReference( 3, 27, 11 ),
	PropReference( 3, 27, 1 ),
	PropReference( 3, 27, 2 ),
	PropReference( 3, 27, 3 ),
	PropReference( 3, 27, 4 ),
	PropReference( 3, 27, 5 ),
	PropReference( 3, 27, 6 ),
	PropReference( 3, 27, 7 ),
	PropReference( 3, 27, 8 ),
	PropReference( 3, 27, 9 ),
	PropReference( 1, 10, 1 ),
	PropReference( 1, 10, 2 ),
	PropReference( 1, 11, 1 ),
	PropReference( 1, 11, 2 ),
	PropReference( 1, 11, 3 ),
	PropReference( 1, 11, 4 ),
	PropReference( 1, 12, 1 ),
	PropReference( 1, 12, 2 ),
	PropReference( 1, 12, 3 ),
	PropReference( 1, 12, 4 ),
	PropReference( 5, 25, 1 ),
	PropReference( 5, 25, 2 ),
	PropReference( 2, 23, 1 ),
	PropReference( 2, 23, 2 ),
	PropReference( 2, 23, 3 ),
	PropReference( 2, 23, 4 ),
	PropReference( 2, 23, 5 ),
	PropReference( 2, 20, 1 ),
	PropReference( 2, 20, 2 ),
	PropReference( 2, 20, 3 ),
	PropReference( 2, 20, 4 ),
	PropReference( 2, 20, 5 ),
	PropReference( 2, 20, 6 ),
	PropReference( 2, 20, 7 ),
	PropReference( 2, 20, 8 ),
};
