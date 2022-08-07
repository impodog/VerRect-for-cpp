#pragma once
#include <map>
#include <list>
#include <string>
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
	SpVR(Pos start, Pos end, bool followable = false, bool gravity=true,float density=1);
	void cspeed(Pos_Like speed);
	void cacc(Pos_Like acc);
	void stop(Pos_Like target);
	void stopx(float target=0.0f);
	void stopy(float target=0.0f);
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
	void lrudt(VerRect *rect, bool *result);
	void lrudt_olrud(VerRect *rect,bool *result);
	void round_lrudt(VerRect *rect, bool *result, Pos_Like oppspeed);
	void stopout(VerRect* rect, bool *result, Pos_Like oppspeed,bool extraleft = true, bool extraright = true, bool extraup = true, bool extradown = true);
	void run();
};