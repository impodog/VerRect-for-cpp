#pragma once
#include <map>
#include <list>
#include <string>
#include <math.h>
#include <regex>
using namespace std;
struct Pos {
	float x;
	float y;
};
typedef Pos Pos_Like;
struct Int_Pos {
	int x;
	int y;
};
Pos create_Pos(float x, float y);
Int_Pos create_Int_Pos(int x, int y);
float get_airfriction();
Pos_Like get_gravity();
string get_version();
void set_airfriction(float value);
void set_gravity(Pos_Like value);
class VerRect {
public:
	Pos start;
	Pos end;
	Pos bef;
	bool followable;
	map<string, list<VerRect*>> *Followed;
	Pos_Like differed;
	void move(float x,float y);
	void absolute(float x = -1,float y = -1);
	void absoluteend(float x = -1, float y = -1);
	void expand(float x, float y = -1);
	Pos_Like ratio(float r);
	VerRect(Pos start, Pos end, bool followable=false);
	VerRect();
	void end_redraw();
	bool contains_VerRect(VerRect *rect);
	bool contains_point(Pos point);
	bool touched(VerRect *rect);
	bool edgetouched(VerRect *rect);
	void follows(VerRect *rect, string followmode="rxry");
	void befollowed(list<VerRect*> rect_list,string followmode="rxry");
	void get_rectvalue(float* value);
	void get_int_rectvalue(int* value);
	Pos get_lb();
	Pos get_ru();
	float get_midx();
	float get_midy();
	Pos get_center();
};
float distance(Pos P1, Pos P2);
Pos sgo(Pos start, Pos end, Pos realstart);
class dire {
public:
	float speed;
	Pos start;
	Pos end;
	float x;
	float y;
	Pos_Like s;
	Int_Pos sym;
	dire(Pos start, Pos end, float speed);
	void turn(bool is_outend, bool is_outstart);
	bool outend(VerRect *rect);
	bool outstart(VerRect *rect);
	void absolute(Pos point);
};
class SpVR :public VerRect {
public:
	Pos_Like speed;
	float density;
	float xface;
	float yface;
	Pos_Like cgbacc;
	float mass;
	Pos_Like clearacc;
	Pos_Like acc;
	bool gravity_switch;
	SpVR();
	SpVR(Pos start, Pos end, bool followable = false, bool gravity=true,float density=1);
	void cspeed(Pos_Like speed);
	void cacc(Pos_Like acc);
	void stop(Pos_Like target);
	void stopx(float target=0);
	void stopy(float target=0);
	void accless_stopx();
	void accless_stopy();
	void acc_stopx();
	void acc_stopy();
	void keepin(VerRect* rect,bool *result);
	void gravswitch(int onoff = -1);
	void run_speed();
	void run();
	void clearacce();
	void clearveracc();
	bool has_no_speed();
	bool has_no_verspeed();
	bool has_no_horispeed();
	bool has_no_acc();
};
class player_SpVR :public SpVR {
public:
	player_SpVR(Pos start, Pos end, bool followable = false, bool gravity=true,float density=1);
	void lrudt(VerRect *rect, bool *result);
	void lrudt_olrud(VerRect *rect,bool *result);
	void round_lrudt(VerRect *rect, bool *result, Pos_Like oppspeed);
	void stopout(VerRect* rect, bool *result, Pos_Like oppspeed,bool extraleft = true, bool extraright = true, bool extraup = true, bool extradown = true);
	void run();
};

#define MINIMUM 1e-6

Pos_Like grav;
float airfriction;
string __version__ = "1.0.2";
float INF = (float)3.40282e+038;


Pos create_Pos(float x, float y) {
	Pos result;
	result.x = x;
	result.y = y;
	return result;
}
Int_Pos create_Int_Pos(int x, int y) {
	Int_Pos result;
	result.x = x;
	result.y = y;
	return result;
}

float get_airfriction() { return airfriction; }
Pos_Like get_gravity() { return grav; }
string get_version() { return __version__; }
void set_airfriction(float value) { airfriction = value; }
void set_gravity(Pos_Like value) { grav = value; }

int symbol(float x) {
	if (x > 0) { return 1; }
	else if (x < 0) { return -1; }
	else { return 0; }
}
int symbol(int x) {
	if (x > 0) { return 1; }
	else if (x < 0) { return -1; }
	else { return 0; }
}
float lower(float x, float y) {
	if (x <= y) { return x; }
	else { return y; }
}
float higher(float x, float y) {
	if (x >= y) { return x; }
	else { return y; }
}
VerRect::VerRect(){
	this->VerRect::VerRect(create_Pos(0, 0), create_Pos(0, 0), false);
}
VerRect::VerRect(Pos start, Pos end, bool followable) {
	this->start = create_Pos(lower(start.x,end.x), lower(start.y, end.y));
	this->end = create_Pos(higher(start.x,end.x),higher(start.y,end.y));
	this->followable = followable;
	this->differed.x = this->end.x - this->start.x;
	this->differed.y = this->end.y - this->start.y;
	this->bef = this->end;
	if (!(this->followable)){
		this->Followed = NULL;
	}
	else {
		this->Followed = new map<string, list<VerRect*>>();
	}
}
void VerRect::move(float x, float y) {
	this->start.x += x;
	this->start.y += y;
	if (this->followable) {
		list<VerRect*>::iterator it;
		for (auto& t : *(this->Followed)) {
			for (it = t.second.begin(); it != t.second.end(); it++) {
				(*it)->move(x, y);
			}
		}
	}
	this->end_redraw();
	
};
void VerRect::absolute(float x, float y) {
	if (x == -1) {
		x = this->start.x;
	}if (y == -1) {
		y = this->start.y;
	}
	float differ_x = x - this->start.x;
	float differ_y = y - this->start.y;
	this->move(differ_x, differ_y);
	if (this->followable) {
		list<VerRect*>::iterator it;
		for (auto& t : *(this->Followed)) {
			for (it = t.second.begin(); it != t.second.end(); it++) {
				(*it)->move(differ_x, differ_y);
			}
		}
	}
};
void VerRect::absoluteend(float x, float y) {
	if (x == -1) {
		x = this->start.x;
	}if (y == -1) {
		y = this->start.y;
	}
	this->absolute(x - this->differed.x, y - this->differed.y);
}
void VerRect::expand(float x, float y) {
	this->bef = this->end;
	list<VerRect*>::iterator it;
	smatch results;
	string pattern_rx("(^r)x");
	regex regex_rx(pattern_rx);
	string pattern_ry("(^r)y");
	regex regex_ry(pattern_ry);
	if (y == -1) {
		Pos_Like rxy = this->ratio(x / this->differed.x + 1);
		for (auto& t : *(this->Followed)) {
			if (t.first.find("rx") != string::npos) {
				for (it = t.second.begin(); it != t.second.end(); it++) {
					(*it)->move(rxy.x, 0);
				}
			}if (t.first.find("ry") != string::npos) {
				for (it = t.second.begin(); it != t.second.end(); it++) {
					(*it)->move(0, rxy.y);
				}
			}if (regex_search(t.first, results, regex_rx)) {
				for (it = t.second.begin(); it != t.second.end(); it++) {
					(*it)->expand(rxy.x, 0);
				}
			}if (regex_search(t.first, results, regex_ry)) {
				for (it = t.second.begin(); it != t.second.end(); it++) {
					(*it)->expand(0, rxy.y);
				}
			}
		}
	}
	else {
		this->differed.x = this->differed.x + x;
		this->differed.y = this->differed.y + y;
		for (auto& t : *(this->Followed)) {
			if (t.first.find("rx") != string::npos) {
				for (it = t.second.begin(); it != t.second.end(); it++) {
					(*it)->move(x, 0);
				}
			}if (t.first.find("ry") != string::npos) {
				for (it = t.second.begin(); it != t.second.end(); it++) {
					(*it)->move(0, y);
				}
			}if (regex_search(t.first, results, regex_rx)) {
				for (it = t.second.begin(); it != t.second.end(); it++) {
					(*it)->expand(x, 0);
				}
			}if (regex_search(t.first, results, regex_ry)) {
				for (it = t.second.begin(); it != t.second.end(); it++) {
					(*it)->expand(0, y);
				}
			}
		}
	}
	this->end_redraw();
}
Pos VerRect::ratio(float r) {
	Pos curend(this->end);
	this->differed.x *= r;
	this->differed.y *= r;
	this->end_redraw();
	return create_Pos(this->end.x - curend.x, this->end.y - curend.y);
}
void VerRect::end_redraw() {
	this->end.x = this->start.x + this->differed.x;
	this->end.y = this->start.y + this->differed.y;
}
bool VerRect::contains_point(Pos point) {
	return (this->start.x < point.x) && (point.x < this->end.x) && (this->start.y < point.y) && (point.y < this->end.y);
}
bool VerRect::contains_VerRect(VerRect *rect) {
	return (this->start.x <= rect->start.x) && (rect->start.x <= this->end.x - rect->differed.x) && (this->start.y <= rect->start.y) && (rect->start.y <= this->end.y - rect->differed.y);
}
bool VerRect::touched(VerRect *rect) {
	return (rect->start.x - this->differed.x <= this->start.x) && (this->start.x <= rect->end.x) && (rect->start.y - this->differed.y <= this->start.y) && (this->start.y <= rect->end.y);
}
bool VerRect::edgetouched(VerRect* rect) {
	return (ceil(rect->start.x - this->differed.x) <= ceil(this->start.x)) && (ceil(this->start.x) <= rect->end.x) && (ceil(rect->start.y - this->differed.y) <= ceil(this->start.y)) && (ceil(this->start.y) <= rect->end.y);
}
void VerRect::follows(VerRect* rect, string followmode) {
/*Follow Mode
x - Change by the x changes(size change)
y - Same as 'x'
r + x or y - Only changes the place, no size changes.*/
	if (!rect->followable) {
		throw false;
	}
	if (rect->Followed->find(followmode) == rect->Followed->end()) {
		rect->Followed->insert(pair<string, list<VerRect*>>(followmode, list<VerRect*>({ this })));
	}
	else {
		list<VerRect*>::iterator it = rect->Followed->find(followmode)->second.end();
		rect->Followed->find(followmode)->second.insert(it, this);
	}
}
void VerRect::befollowed(list<VerRect*> rects,string followmode) {
	if (!this->followable) {
		throw false;
	}
	if (this->Followed->find(followmode) == this->Followed->end()) {
		this->Followed->insert(pair<string, list<VerRect*>>(followmode, list<VerRect*>(rects)));
	}
	else {
		list<VerRect*>::iterator it = this->Followed->find(followmode)->second.end();
		this->Followed->find(followmode)->second.splice(it,rects);
	}
}
void VerRect::get_rectvalue(float* value) {
	value[0] = this->start.x;
	value[1] = this->start.y;
	value[2] = this->differed.x;
	value[3] = this->differed.y;
}
void VerRect::get_int_rectvalue(int* value) {
	value[0] = (int)round(this->start.x);
	value[1] = (int)round(this->start.y);
	value[2] = (int)round(this->differed.x);
	value[3] = (int)round(this->differed.y);
}
Pos VerRect::get_lb() {
	return create_Pos(this->start.x, this->end.y);
}
Pos VerRect::get_ru() {
	return create_Pos(this->end.x, this->start.y);
}
float VerRect::get_midx() {
	return ceil((this->start.x + this->end.x) / 2);
}
float VerRect::get_midy() {
	return ceil((this->start.y + this->end.y) / 2);
}
Pos VerRect::get_center() {
	return create_Pos(this->get_midx(), this->get_midy());
}
float distance(Pos P1, Pos P2) {
	return hypot(P1.x - P2.x, P1.y - P2.y);
}
Pos sgo(Pos start, Pos end, Pos realstart) {
	return create_Pos(realstart.x + end.x - start.x, realstart.y + end.y - start.y);
}
dire::dire(Pos start, Pos end, float speed) {
	this->start = start;
	this->end = end;
	this->speed = speed;
	float xshift = this->end.x - this->start.x;
	float yshift = this->end.y - this->start.y;
	if (xshift == 0) {
		this->x = 0;
		this->y = this->speed * symbol(xshift);
	}
	else {
		float b = (float)(yshift / xshift);
		this->x = (float)(sqrt(pow(this->speed, 2) / (pow(b, 2) + 1)) * symbol(xshift));
		this->y = b * this->x;
	}
	this->s = create_Pos(this->x, this->y);
	this->sym = create_Int_Pos(symbol(this->x), symbol(this->y));
}
void dire::turn(bool outend, bool outstart) {
	if (outend) {
		this->x = (-abs(this->x)) * (this->sym.x);
		this->y = (-abs(this->y)) * (this->sym.y);
	}
	else if (outstart) {
		this->x = (abs(this->x)) * (this->sym.x);
		this->y = (abs(this->y)) * (this->sym.y);
	}
}
bool dire::outend(VerRect *rect) {
	return (rect->start.x * this->sym.x >= this->end.x * this->sym.x) && (rect->start.y * this->sym.y >= this->end.y * this->sym.y);
}
bool dire::outstart(VerRect* rect) {
	return (rect->start.x * this->sym.x <= this->start.x * this->sym.x) && (rect->start.y * this->sym.y <= this->start.y * this->sym.y);
}
void dire::absolute(Pos point) {
	this->end = sgo(this->start, this->end, point);
	this->start = point;
}
SpVR::SpVR(){
	this->SpVR::SpVR(create_Pos(0,0),create_Pos(0,0));
}
SpVR::SpVR(Pos start, Pos end, bool followable, bool gravity, float density) {
	this->VerRect::VerRect(start, end, followable);
	this->speed = create_Pos(0.0f, 0.0f);
	this->density = density;
	this->xface = this->differed.y / 100;
	this->yface = this->differed.x / 100;
	this->cgbacc = create_Pos(0, 0);
	this->gravity_switch = gravity;
	this->mass = this->differed.x * this->differed.y * this->density;
	if (this->gravity_switch) {
		this->clearacc = grav;
	}
	else {
		this->clearacc = create_Pos(0.0f, 0.0f);
	}
	this->acc = this->clearacc;
}
void SpVR::cspeed(Pos_Like speed){
	this->speed.x += speed.x;
	this->speed.y += speed.y;
}
void SpVR::cacc(Pos_Like acc){
	this->acc.x += acc.x;
	this->acc.y += acc.y;
}
void SpVR::stop(Pos_Like target){
	this->stopx(target.x);
	this->stopy(target.y);
}
void SpVR::stopx(float target) {
	this->speed.x = target;
	this->acc.x = this->clearacc.x;
}
void SpVR::stopy(float target) {
	this->speed.y = target;
	this->acc.y = this->clearacc.y;
}
void SpVR::accless_stopx() {
	this->speed.x = 0;
}
void SpVR::accless_stopy() {
	this->speed.y = 0;
}
void SpVR::acc_stopx() {
	this->acc.x = this->clearacc.x;
}
void SpVR::acc_stopy() {
	this->acc.y = this->clearacc.y;
}
void SpVR::keepin(VerRect* rect, bool *result) {
	//At least 4 space
	bool left, right, up, down;
	left = right = up = down = false;
	bool tx, ty;
	tx = ty = true;
	if (this->start.x < rect->start.x) {
		this->absolute(rect->start.x); left = true;
	}
	else if (this->end.x > rect->end.x) {
		this->absoluteend(rect->end.x); right = true;
	}
	else { tx = false; }
	if (this->start.y < rect->start.y) {
		this->absolute(-1.0f, rect->start.y); up = true;
	}
	else if (this->end.y > rect->end.y) {
		this->absoluteend(-1.0f, rect->end.y); down = true;
	}
	else { ty = false; }
	if (tx) { this->stopx(); }
	if (ty) { this->stopy(); }
	result[0] = left; result[1] = right; result[2] = up; result[3] = down;
}
void SpVR::gravswitch(int onoff) {
	if (onoff == -1) {
		this->gravity_switch = !this->gravity_switch;
	}
	else {
		this->gravity_switch = (bool)onoff;
	}
	if (this->gravity_switch) {
		this->clearacc = grav;
	}
	else {
		this->clearacc = create_Pos(0.0f, 0.0f);
	}
	this->acc = this->clearacc;
}
void SpVR::run_speed() {
	this->move(this->speed.x, this->speed.y);
}
void SpVR::run() {
	this->cspeed(create_Pos(-(this->speed.x * airfriction * this->xface), -(this->speed.y * airfriction * this->yface)));
	this->cspeed(create_Pos(this->acc.x + this->cgbacc.x, this->acc.y + this->cgbacc.y));
	this->run_speed();
}
void SpVR::clearacce() {
	this->acc = this->cgbacc = create_Pos(0.0f, 0.0f);
}
void SpVR::clearveracc(){
	this->acc.y = this->clearacc.y;
}
bool SpVR::has_no_speed() { return this->has_no_verspeed() && this->has_no_horispeed(); }
bool SpVR::has_no_verspeed() { return abs(this->speed.y) <= 0.01; }
bool SpVR::has_no_horispeed() { return abs(this->speed.x) <= 0.01; }
bool SpVR::has_no_acc() { return (this->acc.x == 0) && (this->acc.y == 0); }
player_SpVR::player_SpVR(Pos start, Pos end, bool followable, bool gravity, float density){
	this->SpVR::SpVR(start,end,followable,gravity,density);
}
void player_SpVR::lrudt(VerRect* rect,bool *result) {
	//At least 4 space
	result[0] = (rect->start.x - this->differed.x < this->start.x) && (this->start.x < rect->start.x);
	result[1] = (rect->end.x < this->end.x) && (this->end.x < rect->end.x + this->differed.x);
	result[2] = (rect->start.y - this->differed.y < this->start.y) && (this->start.y < rect->start.y);
	result[3] = (rect->end.y < this->end.y) && (this->end.y < rect->end.y + this->differed.y);
	result[4] = this->touched(rect);
}
void player_SpVR::lrudt_olrud(VerRect* rect, bool *result) {
	//At least 9 space
	this->lrudt(rect, result);
	result[5] = (floor(this->end.x) == floor(rect->start.x));
	result[6] = (floor(rect->end.x) == floor(this->start.x));
	result[7] = (floor(this->end.y) == floor(rect->start.y));
	result[8] = (floor(rect->end.y) == floor(this->start.y));
}	
void player_SpVR::round_lrudt(VerRect* rect, bool* result,Pos_Like oppspeed) {
	float x, y;
	float xwent, ywent, total_speed;
	bool rx, ry;
	bool lrudt_result[5];
	this->lrudt(rect, lrudt_result);
	x = y = -1.0f;
	rx = ry = false;
	if (lrudt_result[0]) {
		x = this->end.x - rect->start.x;
	}
	if (lrudt_result[1]) {
		x = rect->end.x - this->start.x;
	}
	if (lrudt_result[2]) {
		y = this->end.y - rect->start.y;
	}
	if (lrudt_result[3]) {
		y = rect->end.y - this->start.y;
	}
	if ((abs(x+1)<=MINIMUM) && (abs(y + 1) > MINIMUM)) {
		ry = true;
	}
	else if ((abs(x + 1) > MINIMUM) && (abs(y + 1) <= MINIMUM)) {
		rx = true;
	}
	else if ((abs(x + 1) > MINIMUM) && (abs(y + 1) > MINIMUM)) {}
	else {
		total_speed = this->speed.x - oppspeed.x;
		if (total_speed != 0) {
			xwent = abs(x / total_speed);
		}
		else {
			xwent = INF;
		}
		total_speed = this->speed.y - oppspeed.y;
		if (total_speed != 0) {
			ywent = abs(y / total_speed);
		}
		else {
			ywent = INF;
		}
		rx = xwent < ywent;
		ry = ywent < xwent;
		if (xwent == ywent){
			rx = ry = true;
		}
	}
	result[0] = rx;
	result[1] = ry;
}
void player_SpVR::stopout(VerRect *rect,bool *result,Pos_Like oppspeed, bool extraleft, bool extraright, bool extraup, bool extradown) {
	bool lrudt_result[9];
	bool round_result[2];
	bool lj, rj, uj, dj;
	bool rleft, rright, rup, rdown;
	rleft = rright = rup = rdown = false;
	this->lrudt_olrud(rect, lrudt_result);
	this->round_lrudt(rect, round_result,oppspeed);

	if (lrudt_result[4]) {
		lj = (this->speed.x > oppspeed.x) && lrudt_result[0] && round_result[0];
		rj = (this->speed.x < oppspeed.x) && lrudt_result[1] && round_result[0];
		uj = (this->speed.y > oppspeed.y) && lrudt_result[2] && round_result[1];
		dj = (this->speed.y < oppspeed.y) && lrudt_result[3] && round_result[1];
		if (!(lrudt_result[5] || lrudt_result[6])) {
			if (uj && extraup){
				this->stopy(oppspeed.y);
				this->absoluteend(-1.0f, rect->start.y);
				rdown = true;
			}
			else if (dj && extradown) {
				this->stopy(oppspeed.y);
				this->absolute(-1.0f, rect->end.y);
				rup = true;
			}
		}
		if (!(lrudt_result[7] || lrudt_result[8])) {
			if (lj && extraleft) {
				this->stopx(oppspeed.x);
				this->absoluteend(rect->start.x);
				rleft = true;
			}
			else if (rj && extraright) {
				this->stopx(oppspeed.x);
				this->absolute(rect->end.x);
				rright = true;
			}
		}
	}
}