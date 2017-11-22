bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
	if (sr == er && sc == ec)  //if the start = end
		return true;

	maze[sr][sc] = 'B'; //where we have been

	if (sr != 0 && maze[sr - 1][sc] != 'X' && maze[sr - 1][sc] != 'B')  //check north
		if (pathExists(maze, sr - 1, sc, er, ec))
			return true;

	if (sc != 9 && maze[sr][sc + 1] != 'X' && maze[sr][sc + 1] != 'B')  //check south
		if (pathExists(maze, sr, sc + 1, er, ec))
			return true;

	if (sr != 9 && maze[sr + 1][sc] != 'X' && maze[sr + 1][sc] != 'B')   //check east
		if (pathExists(maze, sr + 1, sc, er, ec))
			return true;

	if (sc != 0 && maze[sr][sc - 1] != 'X' && maze[sr][sc - 1] != 'B')   //check west
		if (pathExists(maze, sr, sc - 1, er, ec))
			return true;

	return false;
}