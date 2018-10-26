
#pragma  once
enum ErrCode
{
	E_SUCEES = 0,
	E_LoadImage = -1,
	E_Channel = -2,
	E_AreaDlengT = -3,// (area/length) > ãÐÖµ£¬ ÅÐÎªÒì³£
	E_tDtceThTotalLPairpoint = -4,
	E_totalArea = -5,

	E_nonZerosNum = -6,
	E_polysize = -7,

	E_beyongMat,
};
enum lineType
{
  LEFTLINE_TYPE= 0,
  DOWNLINE_TYPE= 1,
  RIGHTLINE_TYPE =2,
  UPLINE_TYPE= 3,
};