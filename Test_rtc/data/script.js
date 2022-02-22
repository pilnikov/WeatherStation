let _from = 'none';
let req_done = true;
let nIntervId;
let sti = 0;
let al;
var numaa = 7;

//---------------------------------------------------------------------------------Time.htm
			function loadVal_Time()
			{
				const event = new Date();
				document.getElementById('date').value  = event.toISOString().slice(0, 10);
				document.getElementById('time').value  = event.toTimeString().slice(0, 8);
				loadVal_Time1();
			}
			
			function loadVal_Time1()
			{
				var xh1 = new XMLHttpRequest();
				xh1.open("GET", "/jacta", true);
				xh1.send(null);
				xh1.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res1 = JSON.parse(xh1.responseText);

    				numaa = res1.actn;

						if (res1.acth < 24 && res1.actm < 59) 
						{
							document.getElementById('act_hour').value = res1.acth;
							document.getElementById('act_min' ).value = res1.actm;
						}
						else 
						{
							document.getElementById('act_hour').value = "--";
							document.getElementById('act_min' ).value = "--";
							numaa = 7;
						}
					loadVal_Time2();
					}
				};
			}
      
			function loadVal_Time2()
			{
  			let dateObj = new Date(0);
				var tz = dateObj.getTimezoneOffset() / -60; 

				var xh2 = new XMLHttpRequest();
				xh2.open("GET", "/jtime", true);
				xh2.send(null);
				xh2.onreadystatechange =  function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh2.responseText);
						document.getElementById('tzone').value = tz;
						document.getElementById('acorr').checked = res.auto_corr;
						document.getElementById('upm').checked = res.upm;
						document.getElementById('nmstart').value = res.nm_start;
						document.getElementById('nmstop').value = res.nm_stop;
						document.getElementById('ehb').checked = res.e_h_b;
						document.getElementById('rtyp').value = res.rtc_t;
						document.getElementById('antp1').value = res.ntp1;
						document.getElementById('antp2').value = res.ntp2;
						document.getElementById('antp3').value = res.ntp3;

						al = res.al;

						document.getElementById('anum').value = numaa;

						if (numaa < 7)
						{
							document.getElementById('atyp'  ).value = al[numaa][0];

							dateObj.setHours(al[numaa][1] + tz, al[numaa][2]);

							var t = dateObj.toISOString().slice(11, 16);
							document.getElementById('atime' ).value = t;
							document.getElementById('amel'  ).value = al[numaa][3];
							document.getElementById('aon'   ).value = al[numaa][4];
							sel_atyp();
						}
					}
				};
				_from = 'time';
			}
/////////////////////////////////////////////////////////////////////////////////////////			

			function sel_anum()
			{
  			let dateObj = new Date(0);
				var tz = dateObj.getTimezoneOffset() / -60; 
				numaa = document.getElementById('anum').value;

				if  (numaa < 7)
				{
					document.getElementById('atyp'  ).value = al[numaa][0];

					dateObj.setHours(al[numaa][1] + tz, al[numaa][2]);

					var t = dateObj.toISOString().slice(11, 16);
					document.getElementById('atime' ).value = t;
					document.getElementById('amel'  ).value = al[numaa][3];
					document.getElementById('aon'   ).value = al[numaa][4];
				}
				sel_atyp();
			}

			function sel_atyp()
			{
				let satyp = document.getElementById('atyp').value;
				let saon  = document.getElementById('aon').value;

				if (numaa > 6) 
				{
					showHide('atyp',  false);
					showHide('atime', false);
					showHide('aon',   false);
					showHide('amel',  false);
				}
				else  
				{
					showHide('atyp',  true);

					if (saon > 0) 
					{
					  showHide('amel',  false);
					}
					else
					{
					  showHide('amel',  true);
					}
	  
					if (satyp > 0)
					{
						showHide('atime', true);
						showHide('aon',   true);
					}
					else
					{
						showHide('atime', false);
						showHide('aon',   false);
						showHide('amel',  false);
					}
				}
			}

			function set_alm()
			{
				if  (numaa < 7)
				{
					al[numaa][0] = document.getElementById('atyp' ).value;
					let dd = document.getElementById('atime').value.split(":");
					al[numaa][1] = dd[0];
					al[numaa][2] = dd[1];
					al[numaa][3] = document.getElementById('amel' ).value;
					al[numaa][4] = document.getElementById('aon'  ).value;
				}
			}
      
			function time_ntp(url)
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) loadVal_Time();
				};     
			}

			function set_time()
			{
				let bb = document.getElementById('date').value.split("-");
				let dd = document.getElementById('time').value.split(":");
				let dateObj = new Date(bb);
				let timeNOW = dateObj.setHours(dd[0], dd[1], dd[2]) / 1000;
				
				let url = '/set_time?in='+timeNOW;

				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
			}
			
			function set_time_2()
			{
				var acorr = 0, upm = 0, ehb = 0, sndpol = 0, ledpol = 0;

				var tzone   = document.getElementById('tzone').value;
				if (document.getElementById('acorr').checked) acorr = 1;
				if (document.getElementById('upm').checked) upm = 1;
				var nmstart = document.getElementById('nmstart').value;
				var nmstop  = document.getElementById('nmstop').value;
				if (document.getElementById('ehb').checked) ehb = 1;
				var srtyp = document.getElementById('rtyp').value;
				var antp1 = document.getElementById('antp1').value;
				var antp2 = document.getElementById('antp2').value;
				var antp3 = document.getElementById('antp3').value;

				let urlc = {
								   tzo: tzone,
							 auto_corr: acorr,
								   upm: upm,
							  nm_start: nmstart,
							   nm_stop: nmstop,
								 e_h_b: ehb,
								 rtc_t: srtyp,
								  ntp1: antp1,
								  ntp2: antp2,
								  ntp3: antp3,
									al: al
							};

				let url = '/set_part?in=' + JSON.stringify(urlc);
				time_ntp(url);				
			}

			function process_Time()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jactt", true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						if (res.actw) 
						{
							document.getElementById('cur_time').value = "Алярма!!!!!!!!";
							loadVal_Time(res.actw);
						}
						else 
  						{
							const event = new Date(res.ct * 1000);
							var tz = event.getTimezoneOffset() * 60000; 
							event.setTime(event.getTime() + tz);
							const options = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric', hour: 'numeric', minute: 'numeric', second: 'numeric'};
							document.getElementById('cur_time').value = event.toLocaleDateString('ru-RU', options);
						}
					}
				};
			}

//-----------------------------------------------------------------------------WiFi.htm
			function loadVal_WiFi()
			{
				let url = '/jwifi';

				if (req_done)
				{
					rec_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							var res = JSON.parse(xh.responseText);
							document.getElementById('aps' ).value   = res.as;
							document.getElementById('app' ).value   = res.ap;
							document.getElementById('ipap').value   = res.iap;
							document.getElementById('maap').value   = res.map;
							document.getElementById('stas1').value  = res.ss1;
							document.getElementById('stap1').value  = res.sp1;
							document.getElementById('sst1').checked = res.st1;
							document.getElementById('stas2').value  = res.ss2;
							document.getElementById('stap2').value  = res.sp2;
							document.getElementById('sst2').checked = res.st2;
							document.getElementById('wof').checked  = res.wof;
	  
				  
							var st = document.getElementById('sst1').checked;
							if (st)
							{
								document.getElementById('ipst1').value = res.ip1;
								document.getElementById('mast1').value = res.ma1;
								document.getElementById('gwst1').value = res.gw1;
								document.getElementById('dns1st1').value = res.d11;
								document.getElementById('dns2st1').value = res.d21;
							}
							st = document.getElementById('sst2').checked;
							if (st)
							{
								document.getElementById('ipst2').value = res.ip2;
								document.getElementById('mast2').value = res.ma2;
								document.getElementById('gwst2').value = res.gw2;
								document.getElementById('dns1st2').value = res.d12;
								document.getElementById('dns2st2').value = res.d22;
							}
							st_ip();
							rec_done = true;
						}
					};
				}
				_from = 'WiFi';
			}
			
			function st_ip()
			{
				var st = document.getElementById('sst1').checked;
				
				showHide('ipst1',   st);
				showHide('mast1',   st);
				showHide('gwst1',   st);
				showHide('dns1st1', st);
				showHide('dns2st1', st);

				st = document.getElementById('sst2').checked;

				showHide('ipst2',   st);
				showHide('mast2',   st);
				showHide('gwst2',   st);
				showHide('dns1st2', st);
				showHide('dns2st2', st);
			}

			function set_wifi()
			{
				var wof = 0, st1= 0, st2 = 0, xh; 

				var as = document.getElementById('aps').value;
				var ap = document.getElementById('app').value;
				var ss1 = document.getElementById('stas1').value;
				var sp1 = document.getElementById('stap1').value;
				var ss2 = document.getElementById('stas2').value;
				var sp2 = document.getElementById('stap2').value;

				if (document.getElementById('sst1').checked) st1 = 1;
				if (document.getElementById('sst2').checked) st2 = 1;

				var iap = document.getElementById('ipap').value;
				var map = document.getElementById('maap').value;

				if (document.getElementById('wof').checked) wof = 1;

				let urlc  = {
							  as: as,
							  ap: ap,
							 iap: iap,
							 map: map,
							 ss1: ss1,
							 sp1: sp1,
							 st1: st1,
							 ss2: ss2,
							 sp2: sp2,
							 st2: st2,
							 wof: wof
							};

				let url = '/set_wifi?in=' + JSON.stringify(urlc);
        
				if (st1 === 1 & st2 === 0) sti = 1;
				if (st1 === 0 & st2 === 1) sti = 2;
				if (st1 === 1 & st2 === 1) sti = 1;
        
				if (req_done)
				{
					rec_done = false;
					xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							req_done = true;
							switch (sti) 
							{
								case 1:
									set_wifi1();
								break;
								case 2:
									set_wifi2();
								break;
								default:
									set_wifi_end();                
								break;
							}
						}
					};
				}
			}

			function set_wifi1()
			{
				var ip = document.getElementById('ipst1').value;
				var ma = document.getElementById('mast1').value;
				var gw = document.getElementById('gwst1').value;
				var d1 = document.getElementById('dns1st1').value;
				var d2 = document.getElementById('dns2st1').value;

				var urlc = {
								ip1: ip,
								ma1: ma,
								gw1: gw,
								d11: d1,
								d21: d2
							};
    
				var url = '/set_ip1?in=' + JSON.stringify(urlc);

				if (req_done)
				{
					req_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200)
						{
							req_done = true;
							switch (sti) 
							{
								case 2:
									set_wifi2();
								break;
								default:
									set_wifi_end();                
								break;
							}
						}
					};
				}
			}

			function set_wifi2()
			{
				var ip = document.getElementById('ipst2').value;
				var ma = document.getElementById('mast2').value;
				var gw = document.getElementById('gwst2').value;
				var d1 = document.getElementById('dns1st2').value;
				var d2 = document.getElementById('dns2st2').value;
		
				var urlc  = {
								ip2: ip,
								ma2: ma,
								gw2: gw,
								d12: d1,
								d22: d2
							};
		
				var url = '/set_ip2?in=' + JSON.stringify(urlc);
        
				if (req_done)
				{
					req_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200)
						{
							req_done = true;
							set_wifi_end();
						}
					};
				}
			}
				

			function set_wifi_end()
			{
				url = '/end_set_wifi';

				if (req_done)
				{
					req_done = false;
					xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							req_done = true;
							loadVal_WiFi();
						}
					};
				}
			}

//------------------------------------------------------------------------------Main
			function process_Main()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jactt", true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						if (res.actw) 
						{
							document.getElementById('cur_time').value = "Алярма!!!!!!!!";
						}
						else 
  						{
							const event = new Date(res.ct * 1000);
							var tz = event.getTimezoneOffset() * 60000; 
							event.setTime(event.getTime() + tz);
							const options = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric', hour: 'numeric', minute: 'numeric', second: 'numeric'};
							document.getElementById('cur_time').value = event.toLocaleDateString('ru-RU', options);
						}
					}
				};
			}

//------------------------------------------------------------------------------Common
			function showHide(element_id, visible) 
			{
				if (document.getElementById(element_id)) //Если элемент с id-шником element_id существует 
				{ 
					var obj = document.getElementById(element_id); //Записываем ссылку на элемент в переменную obj
					if (visible) obj.style.display = "inline-block"; //Показываем элемент
					else obj.style.display = "none"; //Скрываем элемент
				}
				else alert("Элемент с id: " + element_id + " не найден!"); //Если элемент с id-шником element_id не найден, то выводим сообщение
			}   

			function process() 
			{
				if (_from === 'time') //Если элемент с id-шником b_time существует 
				{ 
					process_Time();
				}
				if (_from === 'main') //Если элемент с id-шником b_disp существует 
				{ 
					process_Main();
				}
				req_done = true;
			}   

			nIntervId = setInterval(process, 500);
