let _from = 'main';
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

						if (numaa < 7) 
						{
        			let dateObj = new Date(0);
      				tz = dateObj.getTimezoneOffset() * 60; 
              dateObj.setTime((res1.acta + tz) * 1000);
	  					document.getElementById('cur_atime').value = dateObj.toTimeString().slice(0, 8);
						}
						else 
						{
							document.getElementById('cur_atime').value = "--:--";
							sel_atyp();
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
    				tz = dateObj.getTimezoneOffset() * 60; 
            dateObj.setTime((res.nm_start + tz) * 1000);
						document.getElementById('nmstart').value = dateObj.toTimeString().slice(0, 8);
            dateObj.setTime((res.nm_stop + tz) * 1000);
						document.getElementById('nmstop' ).value = dateObj.toTimeString().slice(0, 8);
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
      				tz = dateObj.getTimezoneOffset() * 60; 
              dateObj.setTime((al[numaa][1] + tz) * 1000);
							document.getElementById('atime' ).value = dateObj.toTimeString().slice(0, 8);
							if(al[numaa][2] < 20)
							{
							  document.getElementById('amel'  ).value = al[numaa][2];
  							document.getElementById('aon'   ).value = 0;
							}
							else
							{
							  document.getElementById('amel'  ).value = 0;
  							document.getElementById('aon'   ).value = al[numaa][2];
							}
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
				numaa = document.getElementById('anum').value;

				if  (numaa < 7)
				{
					document.getElementById('atyp'  ).value = al[numaa][0];
  				let tz = dateObj.getTimezoneOffset() * 60; 
          dateObj.setTime((al[numaa][1] + tz) * 1000);
					document.getElementById('atime' ).value = dateObj.toTimeString().slice(0, 8);
					if(al[numaa][2] < 20)
					{
					  document.getElementById('amel'  ).value = al[numaa][2];
						document.getElementById('aon'   ).value = 0;
					}
					else
					{
					  document.getElementById('amel'  ).value = 0;
						document.getElementById('aon'   ).value = al[numaa][2];
					}
				}
				sel_atyp();
			}

			function sel_atyp()
			{
				if (numaa > 6) 
				{
					showHide('atyp',  false);
					showHide('atime', false);
					showHide('aon',   false);
					showHide('amel',  false);
				}
				else  
				{
  				let satyp = document.getElementById('atyp').value;
	  			let saon  = document.getElementById('aon').value;
		
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
  				let dateObj = new Date(0);
  				let tz = dateObj.getTimezoneOffset() * 60; 
	  			al[numaa][1] = dateObj.setHours(dd[0], dd[1]) / 1000 - tz;

					if(document.getElementById('aon').value < 20)
					{
					  al[numaa][2] = document.getElementById('amel').value;
					}
					else
					{
					  al[numaa][2] = document.getElementById('aon').value;
					}
				}
			}
      
			function time_ntp(url)
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState === 4 && this.status === 200) loadVal_Time();
				};     
			}

			function set_time()
			{
				let bb = document.getElementById('date').value.split("-");
				let dd = document.getElementById('time').value.split(":");
				let dateObj = new Date(bb);
				let tz = dateObj.getTimezoneOffset() * 60; 

				let timeNOW = dateObj.setHours(dd[0], dd[1], dd[2]) / 1000 - tz;
				
				let url = '/set_time?in=' + timeNOW;

        time_ntp(url);
      }
			
			function set_time_2()
			{
				var acorr = 0, upm = 0, ehb = 0, sndpol = 0, ledpol = 0;

				var tzone   = document.getElementById('tzone').value;
				if (document.getElementById('acorr').checked) acorr = 1;
				if (document.getElementById('upm').checked) upm = 1;
				let dateObj = new Date(0);
				let tz = dateObj.getTimezoneOffset() * 60; 
				let dd = document.getElementById('nmstart').value.split(":");
				var nmstart = dateObj.setHours(dd[0], dd[1]) / 1000 - tz;
				    dd = document.getElementById('nmstop').value.split(":");
				var nmstop  = dateObj.setHours(dd[0], dd[1]) / 1000 - tz;
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
        console.log(url);
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
							loadVal_Time();
						}
						else 
						{
							const event = new Date(res.ct *1000);
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
					req_done = false;
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
							req_done = true;
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
					req_done = false;
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

//------------------------------------------------------------------------------Disp.htm
			function loadVal_Disp()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jdisp", true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('ctyp').value = res.ctyp;
						document.getElementById('dtyp').value = res.dtyp;
						document.getElementById('ruslng').checked = res.rlng;
						document.getElementById('tup').checked = res.tup;
						document.getElementById('colu').value = res.colu;
						document.getElementById('cold').value = res.cold;
						document.getElementById('abrd').checked = res.abrd;
						document.getElementById('mbrd').value = res.mbrd;
						document.getElementById('nbrd').value = res.nbrd;
						document.getElementById('brd1').value = res.br_level[0];
						document.getElementById('brd2').value = res.br_level[1];
						document.getElementById('brd3').value = res.br_level[2];
						document.getElementById('brd4').value = res.br_level[3];
						
						show_abr();
					}
				};
				_from = 'disp';
			}

			function process_Disp()
			{
				var xh1 = new XMLHttpRequest();
				xh1.open("GET", "/jactb", true);
				xh1.send(null);
				xh1.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res1 = JSON.parse(xh1.responseText);
						document.getElementById('cur_foto').value = res1.brig;

						var d1 = document.getElementById('mbrd').value;
						document.getElementById('rmbrd').value = d1;

						d1 = document.getElementById('nbrd').value;
						document.getElementById('rnbrd').value = d1;

						d1 = document.getElementById('brd1').value;
						document.getElementById('rbrd1').value = d1;
					  
						d1 = document.getElementById('brd2').value;
						document.getElementById('rbrd2').value = d1;

						d1 = document.getElementById('brd3').value;
						document.getElementById('rbrd3').value = d1;

						d1 = document.getElementById('brd4').value;
						document.getElementById('rbrd4').value = d1;
					}
				};
			}
      
			function set_pard()
			{
				var rlng = 0, abrd = 0, ttup = 0;

				ctyp = document.getElementById('ctyp').value;
				dtyp = document.getElementById('dtyp').value;
				if (document.getElementById('ruslng').checked) rlng = 1;
				if (document.getElementById('tup').checked) ttup = 1;
				if (document.getElementById('abrd').checked) abrd = 1;
				mcou = document.getElementById('colu').value;
				mcod = document.getElementById('cold').value;
				mbrd = document.getElementById('mbrd').value;
				nbrd = document.getElementById('nbrd').value;
				brd1 = document.getElementById('brd1').value;
				brd2 = document.getElementById('brd2').value;
				brd3 = document.getElementById('brd3').value;
				brd4 = document.getElementById('brd4').value;

				var url = '/set_pard?ctyp='+ctyp+'&dtyp='+dtyp+'&rlng='+rlng+'&ttup='+ttup+'&mcou='+mcou+'&mcod='+mcod+'&abrd='+abrd+'&mbrd='+mbrd+'&nbrd='+nbrd+'&brd1='+brd1+'&brd2='+brd2+'&brd3='+brd3+'&brd4='+brd4;

				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
				  if (this.readyState==4 && this.status==200) loadVal_Disp();
				};      
			}
			  
			function change_font()
			{
				tfnt=document.getElementById('tfnt').value;
				url='/set_font?tfnt='+tfnt;

				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
				  if (this.readyState==4 && this.status==200) loadVal_Disp();
				};      
			}

			function show_abr()
			{
				var abrd = 0;

				if (document.getElementById('abrd').checked) abrd = 1;
				ctyp = document.getElementById('ctyp').value;

				if (ctyp != 12) 
				{
					showHide('abrd',  true);
					showHide('mbrd',  true);
					showHide('nbrd',  true);
					showHide('cur_foto',  true);
				}
				else 
				{
					showHide('abrd',  false);
					showHide('mbrd',  false);
					showHide('nbrd',  false);
					showHide('cur_foto',  false);
				}
			  
				if (ctyp != 3) 
				{
					document.getElementById('mbrd').max = 15;
					document.getElementById('nbrd').max = 15;
					document.getElementById('brd3').max = 15;
					document.getElementById('brd4').max = 15;
				}
		 
				if (abrd == 1 && ctyp != 12) 
				{
					showHide('brd1',  true);
					showHide('brd2',  true);
					showHide('brd3',  true);
					showHide('brd4',  true);
				}
				else 
				{
					showHide('brd1',  false);
					showHide('brd2',  false);
					showHide('brd3',  false);
					showHide('brd4',  false);
				}

				dtyp = document.getElementById('dtyp').value;
			 
				if (dtyp < 4) showHide('ruslng',  false);
				else  showHide('ruslng',  true);
				 
				if (dtyp > 21 && dtyp < 30) 
				{
					showHide('colu',  true);
					showHide('cold',  true);
				}
				else 
				{
					showHide('colu',  false);
					showHide('cold',  false);
				}
			}

//------------------------------------------------------------------------------gpio.htm
			function loadVal_GPIO()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jclock", true);
				xh.send(null);
				xh.onreadystatechange =  function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('sndpol').checked = res.sndpol;
						document.getElementById('ledpol').checked = res.ledpol;

						document.getElementById('sda').value = res.sda;
						document.getElementById('scl').value = res.scl;
						document.getElementById('dio').value = res.dio;
						document.getElementById('clk').value = res.clk;
						document.getElementById('dcs').value = res.dcs;
						document.getElementById('dwr').value = res.dwr;
						document.getElementById('trm').value = res.trm;
						document.getElementById('sqw').value = res.sqw;
						document.getElementById('snd').value = res.snd;
						document.getElementById('led').value = res.led;
						document.getElementById('btn').value = res.btn;
						document.getElementById('dht').value = res.dht;
						document.getElementById('ana').value = res.ana;
						document.getElementById('uar').value = res.uar;
					}
				};
				_from = 'gpio';
			}

			function set_parc()
			{
				var acorr = 0, upm = 0, ehb = 0, sndpol = 0, ledpol = 0;

				if (document.getElementById('sndpol').checked) sndpol = 1;
				if (document.getElementById('ledpol').checked) ledpol = 1;

				sda = document.getElementById('sda').value;
				scl = document.getElementById('scl').value;
				dio = document.getElementById('dio').value;
				clk = document.getElementById('clk').value;
				dcs = document.getElementById('dcs').value;
				dwr = document.getElementById('dwr').value;
				trm = document.getElementById('trm').value;
				sqw = document.getElementById('sqw').value;
				snd = document.getElementById('snd').value;
				led = document.getElementById('led').value;
				btn = document.getElementById('btn').value;
				dht = document.getElementById('dht').value;
				ana = document.getElementById('ana').value;
				uar = document.getElementById('uar').value;

				url = '/set_parc?sndpol='+sndpol+'&ledpol='+ledpol+'&sda='+sda+'&scl='+scl+'&dio='+dio+'&clk='+clk+'&dcs='+dcs+'&dwr='+dwr+'&trm='+trm+'&sqw='+sqw+'&snd='+snd+'&led='+led+'&btn='+btn+'&dht='+dht+'&ana='+ana+'&uar='+uar;

				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) loadVal_GPIO();
				};      
			}

//------------------------------------------------------------------------------Main
			function process_Main()
			{
				_from = 'main';
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
							const event = new Date(res.ct *1000);
							var tz = event.getTimezoneOffset() * 60000; 
							event.setTime(event.getTime() + tz);
							const options = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric', hour: 'numeric', minute: 'numeric', second: 'numeric'};
							document.getElementById('cur_time').value = event.toLocaleDateString('ru-RU', options);
						}
					}
				};
			}

//------------------------------------------------------------------------------News
			function loadVal_News()
			{
				var xmlhttp=new XMLHttpRequest();
				xmlhttp.open('GET','https://raw.githubusercontent.com/Qrome/marquee-scroller/master/sources.json',!0);
				xmlhttp.send(null);

				xmlhttp.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var obj=JSON.parse(xmlhttp.responseText);
						obj.sources.forEach(news_src);
					}
				};

				var xh = new XMLHttpRequest();
				xh.open("GET", "/jnews", true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('unews').checked = res.displaynews;
						document.getElementById('newsk').value = res.newsApiKey;
						document.getElementById('newssrc').value = res.newssource;
						
						hide_fields();
					}
				};

				_from = 'news';
			}

			function set_parnews()
			{
				var sdisplaynews = 0;

				if (document.getElementById('unews').checked) sdisplaynews = 1;
				snewsApiKey = document.getElementById('newsk').value;
				snewssource = document.getElementById('newssrc').value;

				url = '/set_news?displaynews='+sdisplaynews+'&newsApiKey='+snewsApiKey+'&newssource='+snewssource;

				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) loadVal_News();
				};     
			}

			function hide_fields()
			{
				 
				unews_t = document.getElementById('unews').checked;
				
				if (unews_t) 
				{
					showHide('newsk'  ,  true);
					showHide('newssrc',  true);
				}
				else 
				{
					showHide('newsk'  ,  false);
					showHide('newssrc',  false);
				}
			}

			  
			function news_src(it)
			{
				var tt;

				if(it!=null)
				{
					tt='<option>'+it.id+'</option>';
					document.getElementById('newssrc').innerHTML+=tt;
				}
			}

			function update_news()
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', '/upd_news', true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
				  if (this.readyState==4 && this.status==200) loadVal_News();
				};      
			}

//------------------------------------------------------------------------------Sens.htm
			function loadVal_Snr()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jsens", true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('cityID').value   = res.cyid;
						document.getElementById('owmk').value     = res.owmk;
						document.getElementById('esaddr1').value  = res.srve1;
						document.getElementById('esaddr2').value  = res.srve2;
						document.getElementById('rdaddr').value   = res.srvr;
						document.getElementById('udp').value      = res.udp;
						document.getElementById('udm').checked    = res.udm;
						document.getElementById('usepp').value    = res.prgp;
						document.getElementById('snr1').value     = res.s1;
						document.getElementById('snr2').value     = res.s2;
						document.getElementById('snr3').value     = res.s3;
						document.getElementById('snrp').value     = res.sp;
						document.getElementById('name_ch1').value = res.nc1;
						document.getElementById('name_ch2').value = res.nc2;
						document.getElementById('name_ch3').value = res.nc3;
						document.getElementById('period').value   = res.peri;
						document.getElementById('esm').checked    = res.esm;
						document.getElementById('uest1').checked  = (res.ess & 0b00000001);
						document.getElementById('uest2').checked  = (res.ess & 0b00000010);
						document.getElementById('uest3').checked  = (res.ess & 0b00000100);
						document.getElementById('uesh1').checked  = (res.ess & 0b00001000);
						document.getElementById('uesh2').checked  = (res.ess & 0b00010000);
						document.getElementById('uesh3').checked  = (res.ess & 0b00100000);
						document.getElementById('uesp').checked   = (res.ess & 0b01000000);

						chk_pp();				  
					}
				};

				var xh2 = new XMLHttpRequest();
				xh2.open("GET", "/jts", true);
				xh2.send(null);
				xh2.onreadystatechange = function()
				{
					if (this.readyState == 4 &&  this.status == 200) 
					{
						var res2 = JSON.parse(xh2.responseText);
						document.getElementById('tschan').value = res2.tsid;
						document.getElementById('tsapir').value = res2.tsar;
						document.getElementById('tsapiw').value = res2.tsaw;
						document.getElementById('utst1').checked = (res2.tss & 0b00000001);
						document.getElementById('utst2').checked = (res2.tss & 0b00000010);
						document.getElementById('utst3').checked = (res2.tss & 0b00000100);
						document.getElementById('utsh1').checked = (res2.tss & 0b00001000);
						document.getElementById('utsh2').checked = (res2.tss & 0b00010000);
						document.getElementById('utsh3').checked = (res2.tss & 0b00100000);
						document.getElementById('utsp').checked  = (res2.tss & 0b01000000);

						sel_ts();
						loadSnr();
					}   	  
				};
				_from = 'sens';
			}
			
			function loadSnr()
			{
				var xh3 = new XMLHttpRequest();
				xh3.open("GET", "/jsnr", true);
				xh3.send(null);
				xh3.onreadystatechange = function()
				{
					if (this.readyState == 4 &&  this.status == 200) 
					{
						var res3 = JSON.parse(xh3.responseText);
						document.getElementById('cur_t1').value = res3.t1;
						document.getElementById('cur_t2').value = res3.t2;
						document.getElementById('cur_t3').value = res3.t3;
						document.getElementById('cur_h1').value = res3.h1;
						document.getElementById('cur_h2').value = res3.h2;
						document.getElementById('cur_h3').value = res3.h3;
						document.getElementById('cur_p' ).value = res3.pres;
					}
				};
			}

			function update_snr()
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', '/upd_snr', true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
				  if (this.readyState==4 && this.status==200) loadSnr();
				};      
			}

			function update_wfc()
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', '/upd_wfc', true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
				  if (this.readyState==4 && this.status==200) loadSnr();
				};      
			}

			function set_pars()
			{
				var udm_t = 0;     
				var cid_t  = document.getElementById('cityID').value;
				var owmk_t = document.getElementById('owmk').value;
				var esa1_t = document.getElementById('esaddr1').value;
				var esa2_t = document.getElementById('esaddr2').value;
				var rda_t  = document.getElementById('rdaddr').value;
				var udp_t  = document.getElementById('udp').value;
				if (document.getElementById('udm').checked) udm_t = 1;
				var upp_t  = document.getElementById('usepp').value;
		 
				var url = '/set_pars1?cid='+cid_t
							 + '&owmk='+owmk_t
							 + '&esa1='+esa1_t
							 + '&esa2='+esa2_t
							 + '&rda=' +rda_t
							 + '&udp=' +udp_t
							 + '&udm=' +udm_t
							 + '&upp=' +upp_t;

				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);

				var ues_t = 0, esm_t = 0;

				snr1_t = document.getElementById('snr1').value;
				snr2_t = document.getElementById('snr2').value;
				snr3_t = document.getElementById('snr3').value;
				snrp_t = document.getElementById('snrp').value;
				period_t = document.getElementById('period').value;
				if (document.getElementById('esm').checked) esm_t = 1;
				if (document.getElementById('uest1').checked) ues_t |= 0b00000001;
				if (document.getElementById('uest2').checked) ues_t |= 0b00000010;
				if (document.getElementById('uest3').checked) ues_t |= 0b00000100;
				if (document.getElementById('uesh1').checked) ues_t |= 0b00001000;
				if (document.getElementById('uesh2').checked) ues_t |= 0b00010000;
				if (document.getElementById('uesh3').checked) ues_t |= 0b00100000;
				if (document.getElementById('uesp').checked)  ues_t |= 0b01000000;
				nc1_t = document.getElementById('name_ch1').value;
				nc2_t = document.getElementById('name_ch2').value;
				nc3_t = document.getElementById('name_ch3').value;
			  
				var url2 = '/set_pars2?snr1='   + snr1_t
						   + '&snr2='   + snr2_t
						   + '&snr3='   + snr3_t
						   + '&snrp='   + snrp_t
						   + '&period=' + period_t
						   + '&ues='    + ues_t
						   + '&esm='    + esm_t
						   + '&nc1='    + nc1_t
						   + '&nc2='    + nc2_t
						   + '&nc3='    + nc3_t;

				var xh1 = new XMLHttpRequest();
				xh1.open('GET', url2, true);
				xh1.send(null);

				var uts_t = 0;

				tschan_t = document.getElementById('tschan').value;
				tsapir_t = document.getElementById('tsapir').value;
				tsapiw_t = document.getElementById('tsapiw').value;
				if (document.getElementById('utst1').checked) uts_t |= 0b00000001;
				if (document.getElementById('utst2').checked) uts_t |= 0b00000010;
				if (document.getElementById('utst3').checked) uts_t |= 0b00000100;
				if (document.getElementById('utsh1').checked) uts_t |= 0b00001000;
				if (document.getElementById('utsh2').checked) uts_t |= 0b00010000;
				if (document.getElementById('utsh3').checked) uts_t |= 0b00100000;
				if (document.getElementById('utsp').checked)  uts_t |= 0b01000000;

				url3 = '/set_pars3?tschan='+tschan_t
							    +'&tsapir='+tsapir_t
							    +'&tsapiw='+tsapiw_t
							    +'&uts='   + uts_t;

				var xh3 = new XMLHttpRequest();
				xh3.open('GET', url3, true);
				xh3.send(null);
				xh3.onreadystatechange = function()
				{
					if (this.readyState==4 && this.status==200) loadVal_Snr();
				};      
			}

			function sel_ts()
			{
				var uts_t = 0, ues_t = 0, udm_t = 0;

				snr1_t = document.getElementById('snr1').value;
				snr2_t = document.getElementById('snr2').value;
				snr3_t = document.getElementById('snr3').value;
				snrp_t = document.getElementById('snrp').value;

/////////////////////////////////// For TS
				if (document.getElementById('utst1').checked) uts_t |= 0b00000001;
				if (document.getElementById('utst2').checked) uts_t |= 0b00000010;
				if (document.getElementById('utst3').checked) uts_t |= 0b00000100;
				if (document.getElementById('utsh1').checked) uts_t |= 0b00001000;
				if (document.getElementById('utsh2').checked) uts_t |= 0b00010000;
				if (document.getElementById('utsh3').checked) uts_t |= 0b00100000;
				if (document.getElementById('utsp').checked)  uts_t |= 0b01000000;
				 
				if (uts_t > 0) showHide('tsapiw',  true);
				else showHide('tsapiw',  false);

				if (snr1_t == 1 || snr2_t == 1 || snr3_t == 1 || snrp_t == 1) showHide('tsapir',  true);
				else showHide('tsapir',  false);

				if (snr1_t == 1 || snr2_t == 1 || snr3_t == 1 || snrp_t == 1 || uts_t > 0) showHide('tschan',  true);
				else showHide('tschan',  false);

/////////////////////////////////// For ES
				if (document.getElementById('uest1').checked) ues_t |= 0b00000001;
				if (document.getElementById('uest2').checked) ues_t |= 0b00000010;
				if (document.getElementById('uest3').checked) ues_t |= 0b00000100;
				if (document.getElementById('uesh1').checked) ues_t |= 0b00001000;
				if (document.getElementById('uesh2').checked) ues_t |= 0b00010000;
				if (document.getElementById('uesh3').checked) ues_t |= 0b00100000;
				if (document.getElementById('uesp').checked)  ues_t |= 0b01000000;

				if (snr1_t == 2 || snr2_t == 2 || snr3_t == 2 || snrp_t == 2 || ues_t > 0) showHide('esaddr1',  true);
				else showHide('esaddr1',  false);

				if (snr1_t == 3 || snr2_t == 3 || snr3_t == 3 || snrp_t == 3) showHide('esaddr2',  true);
				else showHide('esaddr2',  false);

/////////////////////////////////// For UDP
				if (document.getElementById('udm').checked)  udm_t = 1;
				if (udm_t == 1) showHide('udp',  true);
				else showHide('udp',  false);

/////////////////////////////////// For NotUse
				if (snr1_t > 0) 
				{
					showHide('cur_t1',  true);
					showHide('cur_h1',  true);
					showHide('name_ch1',true);
				}
				else 
				{
					showHide('cur_t1',  false);
					showHide('cur_h1',  false);
					showHide('name_ch1',false);
				}

				if (snr2_t > 0) 
				{
					showHide('cur_t2',  true);
					showHide('cur_h2',  true);
					showHide('name_ch2',true);
				}
				else 
				{
					showHide('cur_t2',  false);
					showHide('cur_h2',  false);
					showHide('name_ch2',false);
				}

				if (snr3_t > 0) 
				{
					showHide('cur_t3',  true);
					showHide('cur_h3',  true);
					showHide('name_ch3',true);
				}
				else 
				{
					showHide('cur_t3',  false);
					showHide('cur_h3',  false);
					showHide('name_ch3',false);
				}

				if (snrp_t > 0) 
				{
					showHide('cur_p',  true);
					showHide('cur_p',  true);
				}
				else 
				{
					showHide('cur_p',  false);
					showHide('cur_p',  false);
				}
			}

			function chk_pp()
			{
				usepp_t  = document.getElementById('usepp').value;

				if (usepp_t > 0 ) showHide('cityID',  true);
				else showHide('cityID',  false);

				if (usepp_t == 2 ) showHide('owmk',  true);
				else showHide('owmk',  false);
			}
			  
//------------------------------------------------------------------------------Thermo.htm
			function loadVal_Trm()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jtrm", true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('ittrm').value = res.ttrm;
						document.getElementById('itsrc').value = res.tsrc;
						document.getElementById('idsta').value = res.dsta;
						document.getElementById('idstp').value = res.dstp;
					  
						chk_trs();
					}
				};
				_from = 'thermo';
			}

			function chk_trs()
			{
				 
				ttrm_t   = document.getElementById('ittrm').value;
				if (ttrm_t == 0) 
				{
					showHide('itsrc',  false);
					showHide('idsta',  false);
					showHide('idstp',  false);
				}
				else 
				{
					showHide('itsrc',  true);
					showHide('idsta',  true);
					showHide('idstp',  true);
				}
			}
		  
			function set_partrm()
			{
				ttrm_t = document.getElementById('ittrm').value;
				tsrc_t = document.getElementById('itsrc').value;
				dsta_t = document.getElementById('idsta').value;
				dstp_t = document.getElementById('idstp').value;

				url = '/set_partrm?ttrm=' + ttrm_t
										  + '&tsrc=' + tsrc_t
										  + '&dsta=' + dsta_t
										  + '&dstp=' + dstp_t;

				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) loadValues();
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
				if (_from === 'disp') //Если элемент с id-шником b_disp существует 
				{ 
					process_Disp();
				}
				if (_from === 'main') //Если элемент с id-шником b_disp существует 
				{ 
					process_Main();
				}
				req_done = true;
			}   

      nIntervId = setInterval(process, 500);

