void removeOdds(vector<int>& v)
{
	for (vector<int>::iterator p = v.begin(); p != v.end();)
	{
		if (*p % 2 != 0)
		{
			vector<int>::iterator temp = v.erase(p);
			p = temp;
		}
		else
			p++;
	}
}