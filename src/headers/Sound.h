#pragma once

class Sound {
	private:
		char* data;
		unsigned int sourceid;
	public:
		Sound();
		virtual ~Sound();
		bool isBigEndian();
		int convertToInt(char* buffer,int len);
		char* loadWAV(const char* fn,int& chan,int& samplerate,int& bps,int& size);
		void playSound();
		void init(const char* fn);
};