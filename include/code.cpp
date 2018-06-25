HRESULT CRenderCtrl::RestoreDeviceObjects()
{
	camera.SetProjectionParamters((((FLOAT)m_d3dpp.BackBufferWidth)/((FLOAT)m_d3dpp.BackBufferHeight)),
		0.1f,
		100.0f);

	m_effectList[0].m_Effect->SetTechnique("VertexColor");

	return S_OK;
}



void CRenderCtrl::RenderScene01()
{
	m_offScreenManager.Begin(m_pd3dDevice, m_textureList[3].m_textureSurface);

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         //D3DCOLOR_COLORVALUE(0.5,0.5,0.60,1), 1.0f, 0 );
						 D3DCOLOR_COLORVALUE(0.15,0.5,0.60,1), 1.0f, 0 );

	D3DXVECTOR4 v(1,0,1,1);
	m_effectList[0].m_Effect->SetTexture("diffuseTexture_01", m_textureList[0].m_texture);
	m_effectList[0].m_Effect->SetMatrix("WorldViewProjection_Matrix", &camera.m_ViewProjection);
	m_effectList[0].m_Effect->SetVector("diffuseColor", &v);

	UINT passes = 0;
	m_effectList[0].m_Effect->Begin(&passes, 0);
	for(UINT  ip = 0; ip<passes; ip++)
	{
		m_effectList[0].m_Effect->BeginPass(ip);
		m_geometryList[0].Render(m_pd3dDevice);
		m_effectList[0].m_Effect->EndPass();
	}
	m_effectList[0].m_Effect->End();

	m_offScreenManager.End(m_pd3dDevice);
}