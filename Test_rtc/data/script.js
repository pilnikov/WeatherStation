let _from = 'none';
let req_done = true;
let nIntervId;
let sti = 0;

//---------------------------------------------------------------------------------Time.htm
			function loadVal_Time()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jtime1", true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('hour').value = res.hour;
						document.getElementById('min').value  = res.min;
						document.getElementById('day').value  = res.day;
						document.getElementById('mon').value  = res.month;
						document.getElementById('year').value = res.year;
					}
				};

				var xh2 = new XMLHttpRequest();
				xh2.open("GET", "/jtime2", true);
				xh2.send(null);
				xh2.onreadystatechange =  function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh2.responseText);
						document.getElementById('tzone').value = res.tzon;
						document.getElementById('acorr').checked = res.acor;
						document.getElementById('upm').checked = res.uspm;
						document.getElementById('nmstart').value = res.nstr;
						document.getElementById('nmstop').value = res.nend;
						document.getElementById('ehb').checked = res.evhb;
						document.getElementById('rtyp').value = res.trts;
						document.getElementById('antp1').value = res.antp1;
						document.getElementById('antp2').value = res.antp2;
						document.getElementById('antp3').value = res.antp3;
					}
				}; 

				var xh1 = new XMLHttpRequest();
				xh1.open("GET", "/jalarm", true);
				xh1.send(null);
				xh1.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res1 = JSON.parse(xh1.responseText);
						var num = res1.actn;
						document.getElementById('anum'  ).value = num;

						if  (num < 7)
						{
							document.getElementById('atyp'  ).value = res1.al[num][0];
							document.getElementById('ahour' ).value = res1.al[num][1];
							document.getElementById('amin'  ).value = res1.al[num][2];
							document.getElementById('amel'  ).value = res1.al[num][3];
							document.getElementById('aon'   ).value = res1.al[num][4];
      				sel_atyp();
						}
					}
				};
				_from = 'time';
			}
/////////////////////////////////////////////////////////////////////////////////////////			

			function sel_atyp()
			{
				let satyp = document.getElementById('atyp').value;
			  let saon  = document.getElementById('aon').value;
			  let anum  = document.getElementById('anum').value;

				if (anum > 6) 
				{
				  showHide('atyp',  false);
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
  					showHide('ahour', true);
  					showHide('amin',  true);
  					showHide('aon',   true);
  				}
    			else
  				{
  					showHide('ahour', false);
  					showHide('amin',  false);
  					showHide('aon',   false);
  					showHide('amel',  false);
  				}
        }
			}

			function sel_anum()
			{
				var num = document.getElementById('anum').value;
				if  (num < 7)
				{
					var xh = new XMLHttpRequest();
					xh.open("GET", "/jalarm", true);
					xh.send();
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							var res = JSON.parse(xh.responseText);
							document.getElementById('atyp'  ).value = res.al[num][0];
							document.getElementById('ahour' ).value = res.al[num][1];
							document.getElementById('amin'  ).value = res.al[num][2];
							document.getElementById('amel'  ).value = res.al[num][3];
							document.getElementById('aon'   ).value = res.al[num][4];
      				sel_atyp();
						}
					};
				}
				else
				{
  				sel_atyp();
  			}
			}
		  
			function time_ntp()
			{
				var xh = new XMLHttpRequest();
				xh.open('GET', '/ntp', true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) loadVal_Time();
				};     
			}

			function set_time()
			{
				h=document.getElementById('hour').value;
				m=document.getElementById('min').value;
				d=document.getElementById('day').value;
				mm=document.getElementById('mon').value;
				y=document.getElementById('year').value;
		  
				let urlc  = {
                      h: h,
                      m: m,
                      d: d,
                     mm: mm,
                      y: y
                    };

				let url = '/set_time1?in=' + JSON.stringify(urlc);

		  
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
                     tzone: tzone,
                     acorr: acorr,
                       upm: upm,
                   nmstart: nmstart,
                    nmstop: nmstop,
                       ehb: ehb,
                     srtyp: srtyp,
                     antp1: antp1,
                     antp2: antp2,
                     antp3: antp3
                    };

				let url = '/set_time2?in=' + JSON.stringify(urlc);

				var xh1 = new XMLHttpRequest();
				xh1.open('GET', url, true);
				xh1.send(null);

				var sanum = document.getElementById('anum').value;
				if  (sanum < 7)
				{
					var satyp = document.getElementById('atyp').value;
					var ahour = document.getElementById('ahour').value;
					var amin  = document.getElementById('amin').value;
					var samel = document.getElementById('amel').value;
					var saon  = document.getElementById('aon').value;
							
				  urlc   = {
                     sanum: sanum,
                     satyp: satyp,
                       upm: upm,
                     ahour: ahour,
                      amin: amin,
                     samel: samel,
                      saon: saon
                    };

				  url = '/set_alarm?in=' + JSON.stringify(urlc);
					
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
				}
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
						document.getElementById('cur_time').value = res.tstr;
					}
				};

				var xh1 = new XMLHttpRequest();
				xh1.open("GET", "/jacta", true);
				xh1.send(null);
				xh1.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res1 = JSON.parse(xh1.responseText);

						if (res1.acth < 24 && res1.actm < 59) 
						{
							document.getElementById('act_hour').value = res1.acth;
							document.getElementById('act_min' ).value = res1.actm;
						}
						else 
						{
							document.getElementById('act_hour').value = "--";
							document.getElementById('act_min' ).value = "--";
						}
						if (res1.actw) sel_anum();
					}
				};
			}

///-----------------------------------------------------------------------------WiFi.htm
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
			function loadVal_Main()
			{
				_from = 'main';
			}

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
						document.getElementById('cur_time').value = res.tstr;
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
