<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>">
	<head>
        <title><?php print $head_title ?></title>
        <?php print $head ?>
        <?php print $styles ?>
        <?php print $scripts ?>
        <!--[if IE]><?php print phptemplate_get_ie_styles(); ?><![endif]-->
	</head>
	
    <body<?php print phptemplate_body_class($left, $right); ?>>
        <div id="wrapper">
            <div id="header">
            
                <?php if ($logo): ?>
                <div id="logo">
                <a href="<?php print check_url($front_page); ?>" title="<?php print check_plain($site_name); ?>">
                    <img src="<?php print check_url($logo); ?>" alt="<?php print check_plain($site_name); ?>" class="logo" />
                </a>
                </div>
                <?php endif; ?>
                
                <?php if ($nav): ?>
                <div id="nav">            	
                    <?php print $nav ?>
                </div> <!-- /#nav -->
                <?php endif; ?>
                
                <div id="slogan">
                    <?php
						// If a slogan is written, put in that text, otherwise add the generic image slogan.
                        if ($site_slogan) {
                            print '<div class="sloganText">'. check_plain($site_slogan) .'</div>';
                        } else { ?>
                    		<img src="<?php print base_path() . path_to_theme() ?>/images/layout/sloganBackground-withSlogan.jpg" alt="delmarva's premier provider of quality homes" />
                    <?php } ?>
                    <?php print $breadcrumb; ?>
                </div>
                
            </div> <!-- /#header -->
                
            <div id="container">
				<?php if ($banner && arg(1) == 292): ?>
                    <div id="banner" class="bannerLarge" >
                        <div class="bannerCover">
                            <img src="<?php print base_path() . path_to_theme() ?>/images/layout/bannerContainer.png" alt="Atlantis Homes LLC." />
                        </div>                        
                        <?php print $banner ?>
                    </div>
                <?php endif; ?>
                          
                <div id="containerContent">
                    <?php if ($left): ?>
                      <div id="sidebar-left" class="sidebar">
                        <?php print $left ?>
                      </div> <!-- /#sidebar-left -->
                    <?php endif; ?>
    
                    <div id="center">    
						<?php if ($banner && arg(1) != 292): ?>
                            <div id="banner" class="bannerSmall" >
                                <?php print $banner ?>
                            </div>
                        <?php endif; ?>
                                    
						<?php if ($quickLinks) {
							$centerClass = "withQuickLinks";	
						?>
                          <div id="quickLinks">
                            <?php print $quickLinks ?>
                          </div> <!-- /#quickLinks -->
                        <?php } else {
							$centerClass = "withoutQuickLinks";	
						}
						?>
                        <div id="centerContent" class="<?php print $centerClass; ?>">
                            <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
                            <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block"><ul class="tabs primary">'. $tabs .'</ul>'; endif; ?>
                            <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
                            <?php if ($tabs): print '<span class="clear"></span></div>'; endif; ?>
                            <?php if ($show_messages && $messages): print $messages; endif; ?>
                            <?php print $help; ?>
							<?php if ($contentAbove): ?>
                              <div id="contentAbove" class="centerBlocks">
                                <?php print $contentAbove ?>
                              </div> <!-- /#contentAbove -->
                            <?php endif; ?>
                            
                            <?php print $content ?>
                            
							<?php if ($contentBelow): ?>
                              <div id="contentBelow" class="centerBlocks">
                                <?php print $contentBelow ?>
                              </div> <!-- /#contentBelow -->
                            <?php endif; ?>
                            
                            <!-- Force the container -->
        					<div class="clearBoth">&nbsp;</div>
                    	</div>  <!-- /#centerContent -->
                    </div> <!-- /#center -->
      
                    <?php if ($right): ?>
                      <div id="sidebar-right<?php if(arg(1) == 292): ?>-homepage<?php endif; ?>" class="sidebar">
                        <?php print $right ?>
                      </div> <!-- /#sidebar-right -->
                    <?php endif; ?>
                </div>  <!-- /#containerContent -->
        		<div class="clearBoth">&nbsp;</div>
            </div> <!-- /#container -->
			
            <div id="footer">
            	<?php if ($footer): ?>
            	<div class="footerContent">
					<?php print $footer_message . $footer ?>
                    <?php print $feed_icons ?>
                </div>
                <?php endif; ?>
			</div> <!-- /#footer -->

        </div> <!-- /#wrapper -->
        
        <?php if ($inclind): ?>
        	<div id="inclind">
			<?php print $inclind ?>
            </div>
        <?php endif; ?>
        
        <?php print $closure; ?>
	</body>
</html>
