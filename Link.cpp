
#include <ableton/Link.hpp>
#include <chrono>
#include <thread>
#include <atomic>
#include "SC_Unit.h"
#include "SC_PlugIn.h"

static InterfaceTable *ft;

static ableton::Link *gLink = nullptr;
static float gTempo = 60.0;
static std::thread  gThread;
static std::atomic<bool> gContinueRunningThread(false);

/**
The mutex should be used to modify the thread continuation function, but since
we're using an atomic bool, it is not really needed (except for the cond var)
*/
static std::mutex gThreadMutex;
static std::condition_variable gThreadCond;


static long gDiff = 0;

void* make_link_callback(void* inTempo) {
  ableton::Link link(gTempo);
  link.enable(true);
  gLink = &link;

  
  std::unique_lock<std::mutex> lock(gThreadMutex); // mutex scope lock
  while (gContinueRunningThread.load()) {
      // this releases lock
      gThreadCond.wait(lock);
  }
  gLink = nullptr;
  return nullptr;
}

extern "C" {
  double GetLinkBeat();
  double GetLinkBeatToTime(double beat);
  void SyncUnixTimeWithLink();
  float GetLinkTempo();
}

void SyncUnixTimeWithLink() {
  unsigned long diff = 0;
  for(int i = 0; i < 10; i++) {
    const auto time = gLink->clock().micros();
    unsigned long since_epoch = std::chrono::duration_cast<std::chrono::microseconds>
      (std::chrono::system_clock::now().time_since_epoch()).count() - 0;
    diff += since_epoch - time.count();
  }
  gDiff = diff / 10;
}

double GetLinkBeat() {
  double output = 0.0;
  if (gLink) {
    const auto time = gLink->clock().micros();
    auto timeline = gLink->captureAudioSessionState();
    const auto beats = timeline.beatAtTime(time, 4);
    output =  beats;
  }
  return output;
}

double GetLinkBeatToTime(double beat) {
  double output = 0.0;
  if (gLink) {
    auto timeline = gLink->captureAudioSessionState();
    auto time = timeline.timeAtBeat(beat, 4);
    output = (time.count() + gDiff) * 1e-6;
  }
  return output;
}

float GetLinkTempo() {
  float output = 0.0;
  if (gLink) {
    auto timeline = gLink->captureAudioSessionState();
    output = static_cast<float>(timeline.tempo());
  }
  return output;
}


struct LinkStatus : public Unit {

};

extern "C" {
  void LinkStatus_Ctor(LinkStatus* unit);
  void LinkStatus_next(LinkStatus* unit, int inNumSamples);
}

void LinkStatus_next(LinkStatus* unit, int inNumSamples) {
}

void LinkStatus_Ctor(LinkStatus* unit) {
  if (!gLink) {
    gContinueRunningThread.store(true);
    gTempo = *IN(0);
    gThread = std::thread(make_link_callback, nullptr);
  }
  SETCALC(LinkStatus_next);
}

struct LinkDisabler : public Unit {

};

extern "C" {
    void LinkDisabler_Ctor(LinkDisabler* unit);
    void LinkDisabler_next(LinkDisabler* unit, int inNumSamples);
}

void LinkDisabler_Ctor(LinkDisabler* unit) {
  if (gContinueRunningThread.load()) {
    gContinueRunningThread.store(false);
    gThreadCond.notify_one();
    gThread.join();
  }
  SETCALC(LinkStatus_next);
}

void LinkDisabler_next(LinkStatus* unit, int inNumSamples) {
}



struct Link : public Unit {

};

extern "C" {
  void Link_Ctor(Link* unit);
  void Link_next(Link* unit, int inNumSamples);
}


void Link_Ctor(Link* unit) {
  if (!gLink) {
    Print("warn: Link not enabled!\n");
  }
  SETCALC(Link_next);
}

void Link_next(Link* unit, int inNumSamples) {
  float* output = OUT(0);
  *output = 0.0;
  if (gLink) {
    const auto time = gLink->clock().micros();
    auto timeline = gLink->captureAudioSessionState();
    const auto beats = timeline.beatAtTime(time, 4);
    *output =  static_cast<float>(beats);
  }
}


struct LinkTempo : public Unit {
  double mCurTempo;
  double mTempoCalc;
};

extern "C" {
  void LinkTempo_Ctor(LinkTempo* unit);
  void LinkTempo_next(LinkTempo* unit, int inNumSamples);
}


void LinkTempo_Ctor(LinkTempo* unit) {

  if (!gLink) {
    Print("Link not enabled! can't set Link Tempo!\n");
  }
  
  if (gLink) {
    const auto timeline = gLink->captureAudioSessionState();
    unit->mCurTempo = timeline.tempo();
    unit->mTempoCalc = unit->mCurTempo - *IN(0);
  }
  SETCALC(LinkTempo_next);
}

void LinkTempo_next(LinkTempo* unit, int inNumSamples) {
  if (gLink) {
    auto timeline = gLink->captureAudioSessionState();
    timeline.setTempo(unit->mCurTempo - (*IN(1) * unit->mTempoCalc), gLink->clock().micros());
    gLink->commitAudioSessionState(timeline);
  }
}

PluginLoad(Link) {
  ft = inTable;
  DefineSimpleUnit(LinkStatus);
  DefineSimpleUnit(LinkDisabler);
  DefineSimpleUnit(Link);
  DefineSimpleUnit(LinkTempo);
}

