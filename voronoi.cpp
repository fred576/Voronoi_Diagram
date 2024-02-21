#include <bits/stdc++.h>

using namespace std;

#define watch(x) cout<<"Value of "<<(#x)<<" is "<<x<<endl

struct point{
    int index;
    double x;
    double y;

    bool operator<(const point& p) const{
        if(y!=p.y) return y>p.y;
        return x<p.x;
    }
};
double sweep_line = 0;

struct edge{
    point start;
    point end;
};

point parabola_intersection2(point p1, point p2, double l, int lrd){
    double x1 = p1.x;
    double y1 = p1.y - l;
    double x2 = p2.x;
    double y2 = p2.y - l;

    double a = (y2-y1);
    double b = 2*(x2*y1-x1*y2);
    double c = x1*x1*y2 - x2*x2*y1 + y1*y2*(y1-y2);

    double d = b*b-4*a*c;
    if(d < 0)
        d =0;
    double x_right = (-b+sqrt(d))/(2*a);
    double x_left = (-b-sqrt(d))/(2*a); 

    point ans;
    // watch(x1);
    // watch(y1);
    // watch(x2);
    // watch(y2);
    // watch(a);
    // watch(b);
    // watch(c);
    // watch(d);

    if(lrd == 0)
        ans.x = min(x_left, x_right);
    else if(lrd == 1)
        ans.x =  max(x_right, x_left);

    ans.y = (ans.x*ans.x - 2*ans.x*x1 + x1*x1 + y1*y1 - l*l)/(2*(y1-l));
    return ans;
}
double parabola_intersection(point p1, point p2, double l, int lrd){
    if(lrd == 2)
        return p1.x;
    //cout<<p1.x<<" "<<p1.y<<" "<<p2.x<<" "<<p2.y<<" "<<l<<" "<<lrd<<endl;
    double x1 = p1.x;
    double y1 = p1.y - l;
    double x2 = p2.x;
    double y2 = p2.y - l;

    double a = (y2-y1);
    double b = 2*(x2*y1-x1*y2);
    double c = x1*x1*y2 - x2*x2*y1 + y1*y2*(y1-y2);

    double d = b*b-4*a*c;
    if(d < 0)
        d =0;
    double x_right = (-b+sqrt(d))/(2*a);
    double x_left = (-b-sqrt(d))/(2*a); 

    // watch(x1);
    // watch(y1);
    // watch(x2);
    // watch(y2);
    // watch(a);
    // watch(b);
    // watch(c);
    // watch(d);

    if(lrd == 0)
        return min(x_left, x_right);
    else if(lrd == 1)
        return max(x_right, x_left);
    else{
        cout<<"Error in parabola_intersection"<<endl;
        return -1;
    }
}

struct breakpoint{
    point p1;
    point p2;
    int lrd; //0 for left, 1 for right, 2 for degenerate breakpoint
    point start;

    bool operator<(const breakpoint& b) const{
        double x1,x2;
        x1 = parabola_intersection(p1, p2, sweep_line, lrd);
        x2 = parabola_intersection(b.p1, b.p2, sweep_line, b.lrd);
        if(x1!=x2)
            return x1 < x2;
        else
            return p1.y > b.p1.y;
    }

    bool operator==(const breakpoint& b) const{
        return p1.index == b.p1.index && p2.index == b.p2.index && lrd == b.lrd;
    }
};
point compute_circumcentre(point p1, point p2, point p3){
    double x1 = p1.x;
    double y1 = p1.y;
    double x2 = p2.x;
    double y2 = p2.y;
    double x3 = p3.x;
    double y3 = p3.y;

    cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<x3<<" "<<y3<<endl;

    double a1 = 2*(x2-x1);
    double b1 = 2*(y2-y1);
    double c1 = x2*x2 + y2*y2 - x1*x1 - y1*y1;

    double a2 = 2*(x3-x2);
    double b2 = 2*(y3-y2);
    double c2 = x3*x3 + y3*y3 - x2*x2 - y2*y2;

    double x = (c1*b2 - c2*b1)/(a1*b2 - a2*b1);
    double y = (c1*a2 - c2*a1)/(b1*a2 - b2*a1);

    point p;
    p.x = x;
    p.y = y;
    p.index = -1; //to represent that it's not a input point, but a circumcentre
    return p;
}

double compute_distance(point p1, point p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

void bound_edge(edge& e){
    double bound = 1000;
    if(e.start.x > bound)
        e.start.x = bound;
    if(e.start.x < -bound)
        e.start.x = -bound;
    if(e.start.y > bound)
        e.start.y = bound;
    if(e.start.y < -bound)
        e.start.y = -bound;
    if(e.end.x > bound)
        e.end.x = bound;
    if(e.end.x < -bound)
        e.end.x = -bound;
    if(e.end.y > bound)
        e.end.y = bound;
    if(e.end.y < -bound) 
        e.end.y = -bound;
}
//p2 is ALWAYS the new point on the sweep line
point compute_newHalfedge(point p1, point p2, double l){
    point p;
    p.x = p2.x;
    p.y = (p1.y*p1.y + (p2.x - p1.x)*(p2.x - p1.x) - l*l)/(2*(p1.y - l));
    return p;    
}
struct circle_event{
    breakpoint b1;
    breakpoint b2;
};

struct event_point{
    int is_site;
    point p;
    circle_event c;

    bool operator<(const event_point& e) const{
        if(p.y!=e.p.y) return p.y>e.p.y;
        return p.x<e.p.x;
    }
    
};

multiset<event_point> events;
multiset<breakpoint> beach_line;
vector<edge> edges;

void print_beach_line(){
    cout<<"Beach line is : "<<endl;
    breakpoint testing; 
    for(auto it = beach_line.begin(); it!=beach_line.end(); it++){
        testing = *it;
        cout<<testing.p1.index<<" "<<testing.p2.index<<" "<<testing.lrd<<endl;
    }
    cout<<endl;
}
void delete_circle_events(breakpoint b1, breakpoint b2){
    cout<<"Deleting circle events"<<endl;
    cout<<"b1"<<" "<<b1.p1.index<<" "<<b1.p2.index<<" "<<b1.lrd<<endl;
    cout<<"b2"<<" "<<b2.p1.index<<" "<<b2.p2.index<<" "<<b2.lrd<<endl;
    if(b1.p1.index == b2.p2.index)
        return;
    event_point to_be_deleted;
    to_be_deleted.is_site = 0;
    to_be_deleted.c.b1 = b1;
    to_be_deleted.c.b2 = b2;
    to_be_deleted.p = compute_circumcentre(b1.p1, b1.p2, b2.p2);
    to_be_deleted.p.y -= compute_distance(compute_circumcentre(b1.p1, b1.p2, b2.p2), b1.p1);
    watch(to_be_deleted.p.x);
    watch(to_be_deleted.p.y);
    if(to_be_deleted.p.y > sweep_line)
        return;
    cout<<parabola_intersection(b1.p1, b1.p2, to_be_deleted.p.y, b1.lrd)<<" "<<parabola_intersection(b2.p1, b2.p2, to_be_deleted.p.y, b2.lrd)<<endl;
    if(fabs(parabola_intersection(b1.p1, b1.p2, to_be_deleted.p.y, b1.lrd) - parabola_intersection(b2.p1, b2.p2, to_be_deleted.p.y, b2.lrd)) > 1e-6)
        return;
    
    auto it = events.find(to_be_deleted);
    cout<<"to_be_deleted"<<" "<<to_be_deleted.p.x<<" "<<to_be_deleted.p.y<<endl;
    cout<<"*it"<<" "<<(*it).p.x<<" "<<(*it).p.y<<endl;
    events.erase(it);
    

    cout<<"Circle event deleted"<<endl;
}

void add_circle_event(breakpoint b1, breakpoint b2){
    cout<<"Adding circle event"<<endl;
    cout<<"b1"<<" "<<b1.p1.index<<" "<<b1.p2.index<<" "<<b1.lrd<<endl;
    cout<<"b2"<<" "<<b2.p1.index<<" "<<b2.p2.index<<" "<<b2.lrd<<endl;
    event_point new_circle_event;
    new_circle_event.is_site = 0;
    new_circle_event.p = compute_circumcentre(b1.p1, b1.p2, b2.p2);
    new_circle_event.p.y -= compute_distance(new_circle_event.p, b1.p1);
    new_circle_event.c.b1 = b1;
    new_circle_event.c.b2 = b2;
    if(new_circle_event.p.y > sweep_line)
        return;
    watch(new_circle_event.p.x);
    watch(new_circle_event.p.y);
    cout<<parabola_intersection(b1.p1, b1.p2, new_circle_event.p.y, b1.lrd)<<" "<<parabola_intersection(b2.p1, b2.p2, new_circle_event.p.y, b2.lrd)<<endl;
    if(fabs(parabola_intersection(b1.p1, b1.p2, new_circle_event.p.y, b1.lrd) - parabola_intersection(b2.p1, b2.p2, new_circle_event.p.y, b2.lrd)) > 1e-6)
        return;
    
    events.insert(new_circle_event);
    cout<<"Circle event added with points as x : "<<new_circle_event.p.x<<" y : "<<new_circle_event.p.y<<endl;
}

void handle_site_event(point p){
    breakpoint b;
    b.p1 = p;
    b.p2 = p;
    b.lrd = 2;
    if(beach_line.empty()){
        beach_line.insert(b);
        return;
    }
    else if(beach_line.size() == 1 && beach_line.begin()->lrd == 2){
        breakpoint temp = *beach_line.begin();
        breakpoint b1, b2;
        b1.p1 = temp.p1;
        b1.p2 = p;
        b1.lrd = 0;
        b1.start = compute_newHalfedge(b1.p1, p, sweep_line);
        b2.p1 = p;
        b2.p2 = temp.p1;
        b2.lrd = 1;
        b2.start = b1.start;
        beach_line.erase(beach_line.begin());
        beach_line.insert(b1);
        beach_line.insert(b2);
        return;
    }
    auto it = beach_line.lower_bound(b);
    if(it!=beach_line.end()){
        breakpoint next = *it;
        breakpoint b1,b2;
        b1.p1 = next.p1;
        b1.p2 = p;
        b1.lrd = 0;
        b1.start = compute_newHalfedge(next.p1, p, sweep_line);
        b2.p1 = p;
        b2.p2 = next.p1;
        b2.lrd = 1;
        b2.start = b1.start;

        if(it!=beach_line.begin()){
            breakpoint prev = *(--it);
            cout<<"starting delete "<<endl;
            delete_circle_events(prev, next);
            cout<<"starting to add circle eventss"<<endl;
            add_circle_event(prev, b1);
            add_circle_event(b2, next);
        }
        else
            add_circle_event(b2, next);

        watch(parabola_intersection(b.p1,b.p2,sweep_line,b.lrd));
        watch(parabola_intersection(b1.p1,b1.p2,sweep_line,b1.lrd));
        watch(parabola_intersection(b2.p1,b2.p2,sweep_line,b2.lrd));
        beach_line.insert(b1);
        beach_line.insert(b2);
    }
    else{
        //cout<<"test 2"<<endl;
        auto temp = it;
        temp--;
        breakpoint prev = *temp;
        breakpoint b1, b2;
        b1.p1 = prev.p2;
        b1.p2 = p;
        b1.lrd = 0;
        b2.p1 = p;
        b2.p2 = prev.p2;
        b2.lrd = 1;
        b1.start = compute_newHalfedge(prev.p2, p, sweep_line);
        b2.start = b1.start;
        beach_line.insert(b1);
        beach_line.insert(b2);
        add_circle_event(prev, b1);  
    }
}
//need to handle site event in which x is equal to previous x value already present in beach line

void handle_circle_event(event_point e){
    breakpoint b1 = e.c.b1;
    breakpoint b2 = e.c.b2;

    auto it1 = std::upper_bound(beach_line.begin(), beach_line.end(), b1, [](breakpoint a, breakpoint b) { return a < b; });
    auto it2 = std::upper_bound(beach_line.begin(), beach_line.end(), b2, [](breakpoint a, breakpoint b) { return a < b; });
    
    while(true){
        if(*it1 == b1)
            break;
        else
            it1--;
    }
    while(true){
        if(*it2 == b2)
            break;
        else
            it2--;
    }
    cout<<"b1"<<" "<<b1.p1.index<<" "<<b1.p2.index<<" "<<b1.lrd<<endl;
    cout<<"*it1 "<<(*it1).p1.index<<" "<<(*it1).p2.index<<" "<<(*it1).lrd<<endl;
    cout<<"b2"<<" "<<b2.p1.index<<" "<<b2.p2.index<<" "<<b2.lrd<<endl;
    cout<<"*it2 "<<(*it2).p1.index<<" "<<(*it2).p2.index<<" "<<(*it2).lrd<<endl;
    //cout<<"b1"<<" "<<b1.p1.index<<" "<<b1.p2.index<<" "<<b1.lrd<<endl;
    //cout<<"b2"<<" "<<b2.p1.index<<" "<<b2.p2.index<<" "<<b2.lrd<<endl;
    // //p,q,r,s,t

    point q = b1.p1;
    point r = b1.p2;
    point s = b2.p2;

    breakpoint new_breakpoint;

    new_breakpoint.p1 = q;
    new_breakpoint.p2 = s;

    watch(parabola_intersection(q,s,sweep_line,0));
    watch(e.p.x);

    if(abs(parabola_intersection(q,s,sweep_line,0)  - e.p.x)< 1e-6)
        new_breakpoint.lrd = 0;
    else
        new_breakpoint.lrd = 1;

    if(it1!=beach_line.begin()){
        auto temp = it1;
        temp--;
        breakpoint b0 = *temp;
        point p = b0.p1;
        cout<<"hi"<<endl;
        // print_beach_line();
        // cout << b0.p1.index << " " << b0.p2.index << " " << b0.lrd << endl;
        // cout << b1.p1.index << " " << b1.p2.index << " " << b1.lrd << endl;
        delete_circle_events(b0, b1);
        cout<<"hi2"<<endl;
        add_circle_event(b0, new_breakpoint);
    }
    auto temp = it2;
    temp++;
    auto temp2 = beach_line.end();
    if(temp!=(--temp2) && temp!=beach_line.end()){
        print_beach_line();

        breakpoint b3 = *temp;
        b2 = *it2;
        cout<<"hi3"<<endl;
        
        delete_circle_events(b2, b3);
        cout<<"hi4"<<endl;
        add_circle_event(new_breakpoint, b3);
    }
    cout<<"hi5"<<endl;  
    it1 = std::upper_bound(beach_line.begin(), beach_line.end(), b1, [](breakpoint a, breakpoint b) { return a < b; });
    
    while(true){
        //cout<<(*it1).p1.index<<" "<<(*it1).p2.index<<" "<<(*it1).lrd<<endl;
        if(*it1 == b1)
            break;
        else
            it1--;
    }
    beach_line.erase(it1);
    //cout<<"Reached here"<<endl;
    it2 = std::upper_bound(beach_line.begin(), beach_line.end(), b2, [](breakpoint a, breakpoint b) { return a < b; });
    while(true){
        if(*it2 == b2)
            break;
        else
            it2--;
    }
    beach_line.erase(it2);

    edge new_edge;
    new_edge.start = b1.start;
    new_edge.end = compute_circumcentre(q,r,s);
    edges.push_back(new_edge);
    new_edge.start = b2.start;
    edges.push_back(new_edge);

    new_breakpoint.start = new_edge.end;
    beach_line.insert(new_breakpoint);
    // cout<<"Testinnnnnnn\n";
    // temp = beach_line.find(new_breakpoint);
    // cout<<(*temp).p1.index<<" "<<(*temp).p2.index<<" "<<(*temp).lrd<<endl;
    // temp2 = temp;
    // --temp2;
    // cout<<(*temp2).p1.index<<" "<<(*temp2).p2.index<<" "<<(*temp2).lrd<<endl;
    // cout<<(*temp2 < *(temp))<<endl;
    // watch(parabola_intersection((*temp).p1, (*temp).p2, sweep_line, (*temp).lrd));
    // watch(parabola_intersection((*temp2).p1, (*temp2).p2, sweep_line, (*temp2).lrd));
    // print_beach_line();
}
void bound(){
    for(auto it = beach_line.begin(); it!=beach_line.end(); it++){
        breakpoint b = *it;
        point p;
        p = parabola_intersection2(b.p1, b.p2, sweep_line, b.lrd);
        edge e;
        e.start = b.start;
        e.end = p;
        edges.push_back(e);
    }
}
int main(){
    int n;
    cin>>n;
    point p;
    vector<point> input;

    for(int i=0;i<n; i++){double x,y;cin>>x>>y;
	p.index = i+1;p.x = x;p.y = y; 
    input.push_back(p);
    event_point e;
    e.is_site = 1;e.p = p;
    events.insert(e);
    }
    double previous_sweep_line = -1;
    while(!events.empty()){
        event_point e = *events.begin();
        events.erase(events.begin());   
        sweep_line = e.p.y + 1e-12;
        // if(sweep_line == previous_sweep_line)
        //     continue;
        // previous_sweep_line = sweep_line;
        watch(sweep_line);
        if(e.is_site == 1){
            handle_site_event(e.p);
            print_beach_line();
        }
        else{
            cout<<"Handling circle event with sweep line value "<<sweep_line<<endl;
            handle_circle_event(e);
            print_beach_line();
        }

        watch(events.size());
        // cout<<"Printing events"<<endl;
        // for(auto it = events.begin(); it!=events.end(); it++){
        //     cout<<"*it"<<" "<<(*it).c.b1.p1.index<<" "<<(*it).c.b1.p2.index<<" "<<(*it).c.b1.lrd<<endl;
        //     cout<<"*it"<<" "<<(*it).c.b2.p1.index<<" "<<(*it).c.b2.p2.index<<" "<<(*it).c.b2.lrd<<endl;
        // }
    }
    sweep_line -= 1000;

    bound();

    cout<<"OUTPUTTING"<<endl;
    cout<<input.size()<<endl;
    for(auto v : input){
        cout<<v.x<<" "<<v.y<<endl;
    }
    cout<<edges.size()<<endl;
    for(auto e : edges){
        bound_edge(e);
        cout<<e.start.x<<" "<<e.start.y<<" "<<e.end.x<<" "<<e.end.y<<endl; 
    }
    return 0;
}