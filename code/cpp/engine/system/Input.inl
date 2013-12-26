uint32_t Input::GetScanCode( const char * key ) const
{
	std::unordered_map< uint32_t, uint32_t >::const_iterator it = m_mapStringKey.find(HashString(key));
	if (it == m_mapStringKey.end())
		return 0;
	return it->second;
}

bool Input::IsKeyDownInState( int iState, unsigned int key ) const
{
	return ( m_arrayKeyStates[ iState ][ key ] ) != 0;
}

bool Input::IsKeyDown( const char* key ) const
{
	uint32_t rawKey = GetScanCode(key);
	return IsKeyDownInState( m_iCurrentState^1, rawKey );
}

bool Input::DidKeyJustGoDown( const char* key ) const
{
	unsigned int rawKey = GetScanCode(key);
	return IsKeyDownInState( m_iCurrentState^1, rawKey ) && !IsKeyDownInState( m_iCurrentState, rawKey );
}

bool Input::DidKeyJustGoUp( const char* key ) const
{
	unsigned int rawKey = GetScanCode(key);
	return !IsKeyDownInState( m_iCurrentState^1, rawKey )  && IsKeyDownInState( m_iCurrentState, rawKey );
}

bool Input::IsMouseDown( int btn ) const
{
	return IsMouseDownInState(m_iCurrentState^1, btn);
}

bool Input::DidMouseJustGoDown( int btn ) const
{
	return IsMouseDownInState(m_iCurrentState^1, btn) && !IsMouseDownInState(m_iCurrentState, btn);
}

bool Input::DidMouseJustGoUp( int btn ) const
{
	return !IsMouseDownInState(m_iCurrentState^1, btn) && IsMouseDownInState(m_iCurrentState, btn);
}

bool Input::DidMouseWheelHit( int btn ) const
{
	return m_mouseWheelHit[btn];
}

bool Input::IsPadDownInState(int iState, unsigned int padKey) const
{
	return (m_padButtonState[iState] & padKey) != 0;
}

bool Input::IsPadDown(const char* key) const
{
	unsigned int rawKey = m_padMapStringKey.find(HashString(key))->second;
	return IsPadDownInState(m_iCurrentState ^ 1, rawKey);
}

bool Input::IsPadPress(const char* key) const
{
	unsigned int rawKey = m_padMapStringKey.find(HashString(key))->second;
	return IsPadDownInState(m_iCurrentState ^ 1, rawKey) && !IsPadDownInState(m_iCurrentState, rawKey);
}

bool Input::IsPadRelease(const char* key) const
{
	unsigned int rawKey = m_padMapStringKey.find(HashString(key))->second;
	return !IsPadDownInState(m_iCurrentState ^ 1, rawKey) && IsPadDownInState(m_iCurrentState, rawKey);
}