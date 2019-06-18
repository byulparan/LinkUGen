
#include <ableton/Link.hpp>
#include <chrono>
#include <iostream>
#include "SC_Unit.h"
#include "SC_PlugIn.h"

static InterfaceTable *ft;

static ableton::Link *gLink = nullptr;
static float gTempo = 60.0;

// ========================================================================================================
//
// Link Interface for SuperCollider
//
// ========================================================================================================

struct LinkEnabler : public Unit {
};

extern "C" {
  void LinkEnabler_Ctor(LinkEnabler* unit);
  void LinkEnabler_next(LinkEnabler* unit, int inNumSamples);
}

void LinkEnabler_Ctor(LinkEnabler* unit) {
  if (!gLink) {
    gTempo = *IN(0);
    gLink = new ableton::Link(gTempo);
    gLink->enable(true);
  } else {
    //std::cout<<"Link already running"<<std::endl;
  }
  SETCALC(LinkEnabler_next);
}

void LinkEnabler_next(LinkEnabler* unit, int inNumSamples) {
}


struct LinkDisabler : public Unit {
};

extern "C" {
    void LinkDisabler_Ctor(LinkDisabler* unit);
    void LinkDisabler_next(LinkDisabler* unit, int inNumSamples);
}

void LinkDisabler_Ctor(LinkDisabler* unit) {
  if (gLink) {
    gLink->enable(false);
    delete gLink;
    gLink = nullptr;
  } else {
    //std::cout<<"Link not running"<<std::endl;
  }
  SETCALC(LinkDisabler_next);
}

void LinkDisabler_next(LinkDisabler* unit, int inNumSamples) {
}



struct Link : public Unit {
  float mLastBeat;
};

extern "C" {
  void Link_Ctor(Link* unit);
  void Link_next(Link* unit, int inNumSamples);
}


void Link_Ctor(Link* unit) {
  if (!gLink) {
    Print("warn: Link not enabled!\n");
  }
  unit->mLastBeat = 0.0;
  SETCALC(Link_next);
}

void Link_next(Link* unit, int inNumSamples) {
  float* output = OUT(0);
  if (gLink) {
    const auto time = gLink->clock().micros();
    auto timeline = gLink->captureAudioSessionState();
    const auto beats = timeline.beatAtTime(time, 4);
    *output =  static_cast<float>(beats);
    unit->mLastBeat = *output;
  } else {
    *output = unit->mLastBeat;
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
  } else  {
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


// ========================================================================================================
//
// Application Interface
//
// ========================================================================================================

extern "C" {
  double GetLinkBeat();
  double GetLinkBeatToTime(double beat);
  float GetLinkTempo();
  void SyncUnixTimeWithLink();
}

static long gDiff = 0;

void SyncUnixTimeWithLink() {
  if (gLink) {
    unsigned long diff = 0;
    for(int i = 0; i < 10; i++) {
      const auto time = gLink->clock().micros();
      unsigned long since_epoch = std::chrono::duration_cast<std::chrono::microseconds>
	(std::chrono::system_clock::now().time_since_epoch()).count() - 0;
      diff += since_epoch - time.count();
    }
    gDiff = diff / 10;
  }
}

double GetLinkBeat() {
  double output = 0.0;
  if (gLink) {
    const auto time = gLink->clock().micros();
    auto timeline = gLink->captureAppSessionState();
    const auto beats = timeline.beatAtTime(time, 4);
    output =  beats;
  }
  return output;
}

double GetLinkBeatToTime(double beat) {
  double output = 0.0;
  if (gLink) {
    auto timeline = gLink->captureAppSessionState();
    auto time = timeline.timeAtBeat(beat, 4);
    output = (time.count() + gDiff) * 1e-6;
  }
  return output;
}

float GetLinkTempo() {
  float output = 0.0;
  if (gLink) {
    auto timeline = gLink->captureAppSessionState();
    output = static_cast<float>(timeline.tempo());
  }
  return output;
}

// ========================================================================================================

PluginLoad(Link) {
  ft = inTable;
  DefineSimpleUnit(LinkEnabler);
  DefineSimpleUnit(LinkDisabler);
  DefineSimpleUnit(Link);
  DefineSimpleUnit(LinkTempo);
}

