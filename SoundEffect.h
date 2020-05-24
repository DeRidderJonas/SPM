#pragma once
#include <string>

class Sfx
{
public:
	explicit Sfx( const std::string& path );
	~Sfx( );
	Sfx(const Sfx& other) = delete;
	Sfx& operator=(const Sfx& rhs) = delete;
	Sfx( Sfx&& other) = delete;
	Sfx& operator=( Sfx&& rhs) = delete;

	bool IsLoaded( ) const;
	bool Play( int loops ) const;
	void SetVolume( int value ); 
	int GetVolume( ) const; 
	static void StopAll( );
	static void PauseAll( );
	static void ResumeAll( );

private:
	Mix_Chunk* m_pMixChunk;
};
