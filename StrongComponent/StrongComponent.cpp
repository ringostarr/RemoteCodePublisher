#include <iostream>
#include <stack>
#include <vector>
#include<algorithm>
#include "StrongComponent.h"


//#endif // 

int main()
{

	SSC sc;
	std::vector<node*> v;
	node a, b, c, d, e, f, g, h;
	a.name = "a";
	b.name = "b"; c.name = "c"; d.name = "d"; e.name = "e"; f.name = "f"; g.name = "g"; h.name = "h";
	a.addedge(&e);
	b.addedge(&a);
	c.addedge(&b);
	c.addedge(&d);

	d.addedge(&c);
	e.addedge(&b);
	f.addedge(&b);
	f.addedge(&e);
	f.addedge(&g);
	g.addedge(&c);
	g.addedge(&f);
	h.addedge(&d);
	h.addedge(&g);
	h.addedge(&h);
	v.push_back(&a); v.push_back(&b); v.push_back(&c); v.push_back(&d); v.push_back(&e); v.push_back(&f); v.push_back(&g); v.push_back(&h);



	int index = 1;
	std::stack<node*>* S = new std::stack<node*>;


	for (size_t i = 0; i < v.size(); i++)
	{
		if (v[i]->visited == 0) {

			sc.strongconnect(v[i], index, S);

		}
	}


	return 0;

}

